/******************************************************************************\
 * dbgclnt_proxy.c - Debugger proxy interface routines
 *
 * Copyright 2013-2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <arpa/inet.h>
#include <fcntl.h>
#include <inttypes.h>
#include <libssh2.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "dbg_client.h"
#include "dbgproxy.h"
#include "defs.h"
#include "list.h"
#include "useful.h"

#define START_KEYWORD_FMT "#cray_debug_start"
#define END_KEYWORD_FMT "#cray_debug_end"

// protocol prototypes
static int DbgPROXYInit(void);
static void DbgPROXYFinish(void);
static bitset *DbgPROXYSendCmd(dbginvoke_t *, bitset *, const char *);
static proto_msg_set *DbgPROXYRecvReply(dbginvoke_t *, bitset *, bitset *);
static void DbgPROXYDispatchEvents(dbginvoke_t *, int);
static proto_session *DbgPROXYCreateSession(dbgclnt_proto *, WLM_type manager,
                                            const char *batch_template,
                                            const char *hostname,
                                            const char *username);
static void DbgPROXYSaveSession(proto_session *);
static void DbgPROXYCleanupSession(proto_session *);
static int DbgPROXYAttach(dbginvoke_t *, proto_session *, const char *,
                          const char *, const char *, int, int);
static int DbgPROXYLaunch(dbginvoke_t *, proto_session *, const char *, char **,
                          const char *, char **, const char *, const char *, const char*,
                          char **, const char *, int, int, int, bool);
static int DbgPROXYExit(dbginvoke_t *, int);
static void DbgPROXYDestroyClient(dbginvoke_t *);

// protocol definition
dbgclnt_proto DbgClntPROXYProtoSW = {
    "proxy",                /* sw_name */
    DbgPROXYInit,           /* sw_init */
    DbgPROXYFinish,         /* sw_finish */
    DbgPROXYSendCmd,        /* sw_send_cmd */
    DbgPROXYRecvReply,      /* sw_recv_reply */
    NULL,                   /* sw_checkevents */
    DbgPROXYDispatchEvents, /* sw_dispatchevents */
    DbgPROXYCreateSession,  /* sw_create_session */
    DbgPROXYSaveSession,    /* sw_save_session */
    DbgPROXYCleanupSession, /* sw_cleanup_session */
    DbgPROXYAttach,         /* sw_attach */
    DbgPROXYLaunch,         /* sw_launch */
    DbgPROXYExit,           /* sw_exit */
    DbgPROXYDestroyClient   /* sw_destroyclnt */
};

// Internal prototypes
static int setupPROXYCfgDir(void);
static char *parseBatchScript(const char *, const char *);
static int ssh_start_proxy(const char *, const char *, const char *,
                           const char *);
static proto_msg_set *DbgPROXYRecvProtoMsg(int);
static int cbPROXYAttach(dbginvoke_t *, proto_msg_set *);
static int cbPROXYInvoke(dbginvoke_t *, proto_msg_set *);

// Internal global variables
static int callback_failure = 0;

/*
** Functions start below
*/

static int setupPROXYCfgDir(void) {
    struct stat st;
    char *cfg_dir;

    // grab the environment variable
    if ((cfg_dir = getenv(GUARD_CFG_DIRECTORY)) == NULL) {
        PrintOutError(
            "Could not retrieve %s env var! Please check your environment "
            "settings.\n",
            GUARD_CFG_DIRECTORY);
        return -1;
    }

    // try to stat the directory
    if (stat(cfg_dir, &st) == 0) {
        // the directory already exists, return
        return 0;
    } else {
        // XXX: This gets created in main. This should not fail, but it is good
        // practice just to check
        PrintOutError(
            "Could not find the config directory! Ensure you have write "
            "permissions for %s.\n",
            cfg_dir);
        return -1;
    }

    return 0;
}

// Interface definitions

static int DbgPROXYInit(void) {
    if (setupPROXYCfgDir() == -1) return 0;

    return 1;
}

static void DbgPROXYFinish(void) {
    // what to do here???
}

/*
** Parse a batch job script
*/
static char *parseBatchScript(const char *script_file,
                              const char *launcher_argv_str) {
    FILE *sfp;
    char *cfg_dir;
    char *script_name;
    int ofd;
    FILE *ofp;
    char *line = NULL;
    size_t len = 0;
    int found = 0;

    if ((sfp = fopen(script_file, "r")) == NULL) {
        PrintOutError("ERROR: Cannot open %s.\n", script_file);
        return NULL;
    }

    // allocate script name string
    script_name = (char *)Alloc(MAXPATHLEN);

    // get configuration dir
    if ((cfg_dir = getenv(GUARD_CFG_DIRECTORY)) == (char *)NULL) {
        PrintOutError(
            "ERROR: Cannot find GUARD_CFG_DIRECTORY. Please set MRNet "
            "environment variables!\n");
        Free(script_name);
        return NULL;
    }
    snprintf(script_name, MAXPATHLEN - 1, "%s/%s", cfg_dir, "batchXXXXXX");

    // create a temporary file for the new output script
    if ((ofd = mkstemp(script_name)) < 0) {
        PrintOutError("ERROR: mkstemp failed with %s.\n", script_name);
        Free(script_name);
        return NULL;
    }

    struct stat sb;
    if (fstat(ofd, &sb) == -1) {
        PrintOutError("ERROR: mkstemp failed with %s.\n", script_name);
        Free(script_name);
        return NULL;
    }
    mode_t tempFileMode = sb.st_mode;
    fchmod(ofd, tempFileMode | S_IXUSR);

    // Create a FILE * for this fd
    if ((ofp = fdopen(ofd, "w")) == NULL) {
        PrintOutError("ERROR: Could not fdopen on output script.\n");
        Free(script_name);
        return NULL;
    }

    // process each line in the file
    while (getline(&line, &len, sfp) >= 0) {
        // check to see if this line starts with the start keyword
        if (!strncmp(line, START_KEYWORD_FMT, strlen(START_KEYWORD_FMT))) {
            // found the start keyword!
            ++found;
            // Print out the argv string - note that the newline is needed here
#ifdef DEBUG
            fprintf(ofp, "%s &> proxy_debug.out\n", launcher_argv_str);
#else
            fprintf(ofp, "%s\n", launcher_argv_str);
#endif
            Free(line);
            line = NULL;
            // ignore every line until the end keyword is found or EOF
            while (getline(&line, &len, sfp) >= 0) {
                if (!strncmp(line, END_KEYWORD_FMT, strlen(END_KEYWORD_FMT))) {
                    // done, we should free the line and break out
                    Free(line);
                    line = NULL;
                    break;
                }
                // free the line and continue
                Free(line);
                line = NULL;
            }
        } else {
            // print this line to the file - no newline is needed
            fprintf(ofp, "%s", line);
            Free(line);
            line = NULL;
        }
    }

    // close the temporary file
    if (fclose(ofp)) {
        PrintOutError("ERROR: Could not fclose output script.\n");
        unlink(script_name);
        Free(script_name);
        return NULL;
    }

    // close the script file
    fclose(sfp);

    // ensure we found the line and replaced it, otherwise error out
    if (!found) {
        PrintOutError("ERROR: Could not locate %s line in provided script.\n",
                      START_KEYWORD_FMT);
        PrintOutError(
            "       Wrap the aprun line with %s and %s, then try again.\n",
            START_KEYWORD_FMT, END_KEYWORD_FMT);
        unlink(script_name);
        Free(script_name);
        return NULL;
    }

    return script_name;
}

extern char *cli_get_line(void);

static void interactive_auth_callback(
    const char *name, int name_len, const char *instruction,
    int instruction_len, int num_prompts,
    const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts,
    LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses, void **abstract) {
    int i;
    char *prompt;
    char *pw = NULL;
    (void)name;
    (void)name_len;
    (void)instruction;
    (void)instruction_len;
    (void)abstract;

    if (num_prompts <= 0) {
        return;
    }

    for (i = 0; i < num_prompts; ++i) {
        // print out the prompt
        prompt = (char *)Alloc(prompts[i].length + 1);
        memset(prompt, 0, prompts[i].length + 1);
        strncpy(prompt, prompts[i].text, prompts[i].length);
        PrintOutMsg("\n%s", prompt);
        Free(prompt);

        if ((pw = cli_get_line()) == NULL) {
            return;
        }
        responses[i].text = pw;
        responses[i].length = strlen(pw);
    }

    // Print out final newlines to keep things clean
    PrintOutMsg("\n\n");
}

static int ssh_start_proxy(const char *hostname, const char *username,
                           const char *lhost, const char *lport) {
    int auth = 0;
    struct addrinfo hints;
    struct addrinfo *host = NULL;
    int rc, ssh_sock;
    LIBSSH2_SESSION *session = NULL;
    LIBSSH2_KNOWNHOSTS *nh = NULL;
    const char *fingerprint;
    size_t len;
    int type;
    char *userauthlist;
    LIBSSH2_AGENT *agent = NULL;
    LIBSSH2_CHANNEL *channel;
    char *proxy_script;
    char *guard_proxy_loc;
    FILE *sfp;
    char *cfg_dir;
    char *script_name;
    int ofd;
    FILE *ofp;
    char *line = NULL;
    struct stat st;
    char mem[1024];
    size_t nread;
    char *ptr;
    struct passwd *pw;
    char *remote_file;
    char *cmd_str;

    // Setup the hints structure
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;

    // get the addrinfo on the host
    if ((rc = getaddrinfo(hostname, "22", &hints, &host)) != 0) {
        PrintOutError("Getaddrinfo failed: %s\n", gai_strerror(rc));
        return 1;
    }

    // init the ssh library
    if ((rc = libssh2_init(0))) {
        PrintOutError("libssh2 initialization failed (%d).\n", rc);
        freeaddrinfo(host);
        return 1;
    }

    // create the socket
    if ((ssh_sock = socket(host->ai_family, host->ai_socktype,
                           host->ai_protocol)) < 0) {
        PrintOutError("host socket creation failed.\n");
        freeaddrinfo(host);
        libssh2_exit();
        return 1;
    }

    // Connect the socket
    if (connect(ssh_sock, host->ai_addr, host->ai_addrlen)) {
        PrintOutError("Failed to connect to host %s!\n", hostname);
        freeaddrinfo(host);
        libssh2_exit();
        return 1;
    }

    // cleanup
    freeaddrinfo(host);

    // Create a session instance and start it up. This will trade welcome
    // banners, exchange keys, and setup crypto, compression, and MAC layers
    session = libssh2_session_init();
    if (libssh2_session_handshake(session, ssh_sock)) {
        PrintOutError("Failure establishing SSH session.\n");
        libssh2_session_disconnect(session, "Error occured.");
        libssh2_session_free(session);
        close(ssh_sock);
        libssh2_exit();
        return 1;
    }

    // At this point we havn't authenticated. The first thing to do is check
    // the hostkey's fingerprint against our known hosts.

    // init knownhost file
    if ((nh = libssh2_knownhost_init(session)) == NULL) {
        PrintOutError("Failure initializing knownhost file.\n");
        libssh2_session_disconnect(session, "Error occured.");
        libssh2_session_free(session);
        close(ssh_sock);
        libssh2_exit();
        return 1;
    }

    // read all hosts from here
    libssh2_knownhost_readfile(nh, "known_hosts",
                               LIBSSH2_KNOWNHOST_FILE_OPENSSH);

    fingerprint = libssh2_session_hostkey(session, &len, &type);

    if (fingerprint) {
        struct libssh2_knownhost *host;
        int check;

        check = libssh2_knownhost_check(
            nh, hostname, fingerprint, len,
            LIBSSH2_KNOWNHOST_TYPE_PLAIN | LIBSSH2_KNOWNHOST_KEYENC_BASE64,
            &host);

        switch (check) {
            case LIBSSH2_KNOWNHOST_CHECK_MATCH:
                // Do nothing
                break;

            case LIBSSH2_KNOWNHOST_CHECK_NOTFOUND:
                // Add the host to the host file and continue
                libssh2_knownhost_addc(nh, hostname, NULL, fingerprint, len,
                                       NULL, 0,
                                       LIBSSH2_KNOWNHOST_TYPE_PLAIN |
                                           LIBSSH2_KNOWNHOST_KEYENC_BASE64 |
                                           LIBSSH2_KNOWNHOST_KEY_SSHRSA,
                                       NULL);
                break;

            case LIBSSH2_KNOWNHOST_CHECK_MISMATCH:
                PrintOutError(
                    "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
                    "@\n");
                PrintOutError(
                    "@    WARNING: REMOTE HOST IDENTIFICATION HAS CHANGED!     "
                    "@\n");
                PrintOutError(
                    "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
                    "@\n");
                PrintOutError(
                    "IT IS POSSIBLE THAT SOMEONE IS DOING SOMETHING NASTY!\n");
                PrintOutError(
                    "Someone could be eavesdropping on you right now "
                    "(man-in-the-middle attack)!");
                PrintOutError(
                    "It is also possible that the RSA host key has just been "
                    "changed.");
                PrintOutError(
                    "The fingerprint for the RSA key sent by the remote host "
                    "is");
                PrintOutError("%s.\n", host->key);
                PrintOutError("Please contact your system administrator.\n");
                goto mismatch_exit;

            case LIBSSH2_KNOWNHOST_CHECK_FAILURE:
            default:
                PrintOutError("Failure with ssh knownhost check!\n");
            mismatch_exit:
                libssh2_knownhost_free(nh);
                libssh2_session_disconnect(session, "Error occured.");
                libssh2_session_free(session);
                close(ssh_sock);
                libssh2_exit();
                return 1;
        }
    } else {
        PrintOutError("Failure with obtaining ssh remote host key!\n");
        libssh2_knownhost_free(nh);
        libssh2_session_disconnect(session, "Error occured.");
        libssh2_session_free(session);
        close(ssh_sock);
        libssh2_exit();
        return 1;
    }
    libssh2_knownhost_free(nh);

    // check what authentication methods are available
    userauthlist = libssh2_userauth_list(session, username, strlen(username));

    // Check to see if we can use the passwordless login method, otherwise
    // ensure we can use interactive login
    if (strstr(userauthlist, "publickey") != NULL) {
        // Start by trying to use the ssh-agent mechanism
        struct libssh2_agent_publickey *identity, *prev_identity = NULL;

        // Connect to the ssh-agent
        if ((agent = libssh2_agent_init(session)) == NULL) {
            PrintOutError("Warning: Could not init ssh-agent support.\n");
            goto keyfile_fallback;
        }

        if (libssh2_agent_connect(agent)) {
            PrintOutError("Warning: Could not connect to ssh-agent.\n");
            goto keyfile_fallback;
        }

        if (libssh2_agent_list_identities(agent)) {
            PrintOutError(
                "Warning: Could not request identities from ssh-agent.\n");
            goto keyfile_fallback;
        }

        while (1) {
            rc = libssh2_agent_get_identity(agent, &identity, prev_identity);
            if (rc == 1) break;

            if (rc < 0) {
                PrintOutError(
                    "Warning: Could not obtain identity from ssh-agent "
                    "support.\n");
                rc = 1;
                break;
            }

            if (libssh2_agent_userauth(agent, username, identity) == 0) {
                break;
            }
            prev_identity = identity;
        }

        // Done with the agent so we can close it
        libssh2_agent_disconnect(agent);
        libssh2_agent_free(agent);

        if (rc) {
            // Failed to use the ssh-agent, try to use the default keyfiles
            // instead.
            char *priv_path;
            char *pub_path;

        keyfile_fallback:

            // Get the home directory name
            pw = getpwuid(getuid());

            // Create the path to the private key rsa file
            Asprintf(&priv_path, "%s/.ssh/id_rsa", pw->pw_dir);

            // Try to stat it
            if (stat(priv_path, &st)) {
                // it doesn't exist, try the dsa method instead
                Free(priv_path);

                // Create the path to the private key dsa file
                Asprintf(&priv_path, "%s/.ssh/id_dsa", pw->pw_dir);

                // Try to stat it
                if (stat(priv_path, &st)) {
                    // Failed to find either rsa or dsa mechanism. Failure
                    Free(priv_path);
                    PrintOutError(
                        "Could not authenticate via publickey method.\n");
                    goto publickey_failure;
                }

                // Found the private key, create the public key file and try to
                // stat it
                Asprintf(&pub_path, "%s/.ssh/id_dsa.pub", pw->pw_dir);
                if (stat(pub_path, &st)) {
                    // Found the private key, failed to find the public key
                    Free(priv_path);
                    Free(pub_path);
                    PrintOutError(
                        "Located id_dsa, failed to locate id_dsa.pub!\n");
                    PrintOutError(
                        "Could not authenticate via publickey method.\n");
                    goto publickey_failure;
                }

                // Try authenticating with an empty passphrase - TODO: Do all
                // sites only use empty passphrases???
                if (libssh2_userauth_publickey_fromfile(
                        session, username, pub_path, priv_path, "")) {
                    // Failed to authenticate
                    PrintOutError("id_dsa authentication failed.\n");
                    PrintOutError(
                        "Could not authenticate via publickey method.\n");
                    goto publickey_failure;
                }
            } else {
                // Found the private key, create the public key file and try to
                // stat it
                Asprintf(&pub_path, "%s/.ssh/id_rsa.pub", pw->pw_dir);
                if (stat(pub_path, &st)) {
                    // Found the private key, failed to find the public key
                    Free(priv_path);
                    Free(pub_path);
                    PrintOutError(
                        "Located id_rsa, failed to locate id_rsa.pub!\n");
                    PrintOutError(
                        "Could not authenticate via publickey method.\n");
                    goto publickey_failure;
                }

                // Try authenticating with an empty passphrase - TODO: Do all
                // sites only use empty passphrases???
                if (libssh2_userauth_publickey_fromfile(
                        session, username, pub_path, priv_path, "")) {
                    // Failed to authenticate
                    PrintOutError("id_rsa authentication failed.\n");
                    PrintOutError(
                        "Could not authenticate via publickey method.\n");
                    goto publickey_failure;
                }
            }

            // cleanup
            Free(priv_path);
            Free(pub_path);
        }

        // We're authenticated now
        ++auth;
    }

publickey_failure:

    // Try to use keyboard-interactive if not already authed
    if (!auth && strstr(userauthlist, "keyboard-interactive") != NULL) {
        PrintOutMsg("Performing keyboard-interactive authentication.\n");

        if (libssh2_userauth_keyboard_interactive(session, username,
                                                  &interactive_auth_callback)) {
            // Failed to authenticate
            PrintOutError("keybaord-interactive authentication failed.\n");
            goto keyinter_failure;
        }

        // we're authenticated now
        ++auth;
    }

keyinter_failure:

    // Ensure we are now authed
    if (!auth) {
        // No known authentication methods found!
        PrintOutError("No supported ssh authentication method found.\n");
        libssh2_session_disconnect(session, "Error occured.");
        libssh2_session_free(session);
        close(ssh_sock);
        libssh2_exit();
        return 1;
    }

    // search for start_proxy.sh in path
    if ((proxy_script = pathFind("start_proxy.sh", NULL)) == NULL) {
        PrintOutError("Could not locate the start_proxy.sh script in path.\n");
        libssh2_session_disconnect(session, "Error occured.");
        libssh2_session_free(session);
        close(ssh_sock);
        libssh2_exit();
        return 1;
    }

    // open the script for reading
    if ((sfp = fopen(proxy_script, "r")) == NULL) {
        PrintOutError("ERROR: Cannot open %s.\n", proxy_script);
        Free(proxy_script);
        libssh2_session_disconnect(session, "Error occured.");
        libssh2_session_free(session);
        close(ssh_sock);
        libssh2_exit();
        return 1;
    }

    // cleanup
    Free(proxy_script);

    // allocate script name string
    script_name = (char *)Alloc(MAXPATHLEN);

    // get configuration dir
    if ((cfg_dir = getenv(GUARD_CFG_DIRECTORY)) == (char *)NULL) {
        PrintOutError(
            "ERROR: Cannot find %s. Please set MRNet environment variables!\n",
            GUARD_CFG_DIRECTORY);
        Free(script_name);
        libssh2_session_disconnect(session, "Error occured.");
        libssh2_session_free(session);
        close(ssh_sock);
        libssh2_exit();
        return 1;
    }
    snprintf(script_name, MAXPATHLEN - 1, "%s/%s", cfg_dir, "proxyXXXXXX");

    // create a temporary file for the new output script
    if ((ofd = mkstemp(script_name)) < 0) {
        PrintOutError("ERROR: mkstemp failed with %s.\n", script_name);
        Free(script_name);
        libssh2_session_disconnect(session, "Error occured.");
        libssh2_session_free(session);
        close(ssh_sock);
        libssh2_exit();
        return 1;
    }

    struct stat sb;
    if (fstat(ofd, &sb) == -1) {
        PrintOutError("ERROR: mkstemp failed with %s.\n", script_name);
        Free(script_name);
        return 1;
    }
    mode_t tempFileMode = sb.st_mode;
    fchmod(ofd, tempFileMode | S_IXUSR);

    // Create a FILE * for this fd
    if ((ofp = fdopen(ofd, "w")) == NULL) {
        PrintOutError("ERROR: Could not fdopen on output script.\n");
        Free(script_name);
        libssh2_session_disconnect(session, "Error occured.");
        libssh2_session_free(session);
        close(ssh_sock);
        libssh2_exit();
        return 1;
    }

    // copy each line in the file to the temporary file
    while (getline(&line, &len, sfp) >= 0) {
        // print this line to the file - no newline is needed
        fprintf(ofp, "%s", line);
        Free(line);
        line = NULL;
    }

    // Find the guard_proxy binary in path, we will launch this directly
    if ((guard_proxy_loc = pathFind("guard_proxy", NULL)) == NULL) {
        PrintOutError("ERROR: Could not locate guard_proxy in PATH.\n");
        Free(script_name);
        libssh2_session_disconnect(session, "Error occured.");
        libssh2_session_free(session);
        close(ssh_sock);
        libssh2_exit();
        return 1;
    }

    // write the proxy start command to the file
#ifdef DEBUG
    fprintf(ofp, "%s -h %s -p %s &> proxy_debug.out\n", guard_proxy_loc, lhost,
            lport);
#else
    fprintf(ofp, "%s -h %s -p %s\n", guard_proxy_loc, lhost, lport);
#endif

    // flush the output file
    fflush(ofp);

    // cleanup
    Free(guard_proxy_loc);

    // close the temporary file
    if (fclose(ofp)) {
        PrintOutError("ERROR: Could not fclose output script.\n");
        unlink(script_name);
        Free(script_name);
        libssh2_session_disconnect(session, "Error occured.");
        libssh2_session_free(session);
        close(ssh_sock);
        libssh2_exit();
    }

    // close the script file
    fclose(sfp);

    // Add execute permission to the file
    chmod(script_name, S_IRWXU);

    // send the script_name over to the other end using scp because eslogin
    // nodes tend to be f*cked up in house and they don't cross mount home like
    // they should.
    stat(script_name, &st);

    // Get the home directory name
    pw = getpwuid(getuid());
    // create the remote file string
    Asprintf(&remote_file, "%s/launch_proxy.sh", pw->pw_dir);

    // Send a file via scp. The mode parameter must only have permissions!
    if ((channel = libssh2_scp_send(session, remote_file, st.st_mode & 0777,
                                    (unsigned long)st.st_size)) == NULL) {
        char *errmsg;
        int errlen;
        int err = libssh2_session_last_error(session, &errmsg, &errlen, 0);
        PrintOutError("Unable to open a channel: (%d) %s\n", err, errmsg);
        unlink(script_name);
        Free(script_name);
        Free(remote_file);
        libssh2_session_disconnect(session, "Error occured.");
        libssh2_session_free(session);
        close(ssh_sock);
        libssh2_exit();
        return 1;
    }

    sfp = NULL;
    if ((sfp = fopen(script_name, "rb")) == NULL) {
        PrintOutError("Unable to open file %s\n", script_name);
        unlink(script_name);
        Free(script_name);
        Free(remote_file);
        libssh2_session_disconnect(session, "Error occured.");
        libssh2_session_free(session);
        close(ssh_sock);
        libssh2_exit();
        return 1;
    }

    do {
        nread = fread(mem, 1, sizeof(mem), sfp);
        if (nread <= 0) {
            // end of file
            break;
        }

        ptr = mem;

        do {
            // write the same data over and over, until error or completion
            rc = libssh2_channel_write(channel, ptr, nread);
            if (rc < 0) {
                PrintOutError("Error %d\n", rc);
                unlink(script_name);
                Free(script_name);
                Free(remote_file);
                libssh2_session_disconnect(session, "Error occured.");
                libssh2_session_free(session);
                close(ssh_sock);
                libssh2_exit();
                return 1;
            } else {
                // rc indicates how many bytes were written this time
                ptr += rc;
                nread -= rc;
            }
        } while (nread);
    } while (1);

    // send EOF
    libssh2_channel_send_eof(channel);

    // Wait EOF
    libssh2_channel_wait_eof(channel);

    // close channel
    libssh2_channel_wait_closed(channel);
    libssh2_channel_free(channel);
    channel = NULL;

    // remove the proxy file
    unlink(script_name);
    Free(script_name);

    // Open a channel to the remote host
    if ((channel = libssh2_channel_open_session(session)) == NULL) {
        PrintOutError("Could not open ssh channel.\n");
        Free(remote_file);
        libssh2_session_disconnect(session, "Error occured.");
        libssh2_session_free(session);
        close(ssh_sock);
        libssh2_exit();
        return 1;
    }

    // construct the command string
    Asprintf(&cmd_str, "nohup %s > /dev/null 2> /dev/null < /dev/null &",
             remote_file);
    // Asprintf(&cmd_str, "nohup %s &", remote_file);
    Free(remote_file);

    DPRINT("Sending command: %s\n", cmd_str);

    if (libssh2_channel_exec(channel, cmd_str)) {
        PrintOutError("Failed to launch guard_proxy.\n");
        libssh2_session_disconnect(session, "Error occured.");
        libssh2_session_free(session);
        close(ssh_sock);
        libssh2_exit();
        Free(cmd_str);
        return 1;
    }

    libssh2_channel_close(channel);
    libssh2_channel_free(channel);
    libssh2_session_disconnect(session, "Normal shutdown.");
    libssh2_session_free(session);
    close(ssh_sock);
    libssh2_exit();
    Free(cmd_str);

    return 0;
}

/*
** Create a new proxy session
** manager: workload manager type
** All arguments are optional dependent on manager.
*/
static proto_session *DbgPROXYCreateSession(dbgclnt_proto *proto,
                                            WLM_type manager,
                                            const char *batch_template,
                                            const char *hostname,
                                            const char *username) {
    proto_session *rtn;
    proxy_session *sess;
    struct addrinfo hints;
    struct addrinfo *listener = NULL;
    int rc, sopt;
    char lhost[MAXHOSTNAMELEN];
    char *lport;

    // check the args to ensure they are valid
    switch (manager) {
        case WLM_none:
            if (hostname == NULL) {
                PrintOutError("Hostname missing for session.\n");
                return NULL;
            }
            if (username == NULL) {
                // set the username automatically for the user
                struct passwd *pw;

                pw = getpwuid(getuid());
                username = pw->pw_name;
            }
            break;

        case WLM_qsub:
        case WLM_sbatch:
            if (batch_template == NULL) {
                PrintOutError("Batch template missing for session.\n");
                return NULL;
            }
            break;
    }

    // Create a new session object
    sess = NewProxySession();

    DPRINT("Opening proxy listen socket.\n");

    // Setup the hints structure
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

    // get the addrinfo on my host
    if ((rc = getaddrinfo(NULL, "0", &hints, &listener)) != 0) {
        PrintOutError("Getaddrinfo failed: %s\n", gai_strerror(rc));
        CleanupProxySession(sess);
        return NULL;
    }

    // create the socket
    if ((PROXY_SESSION_LISTENFD(sess) =
             socket(listener->ai_family, listener->ai_socktype,
                    listener->ai_protocol)) < 0) {
        PrintOutError("Listener socket creation failed.\n");
        CleanupProxySession(sess);
        freeaddrinfo(listener);
        return NULL;
    }

    // bind to the socket
    if (bind(PROXY_SESSION_LISTENFD(sess), listener->ai_addr,
             listener->ai_addrlen) == -1) {
        PrintOutError("Listener bind on socket failed.\n");
        CleanupProxySession(sess);
        freeaddrinfo(listener);
        return NULL;
    }

    // cleanup the listener object, we are done with it
    freeaddrinfo(listener);

    // listen on the socket
    if (listen(PROXY_SESSION_LISTENFD(sess), 3) < 0) {
        PrintOutError("Listen failed.\n");
        CleanupProxySession(sess);
        return NULL;
    }

    // get the sockaddr of the listener socket since we called bind with "0"
    if (getsockname(PROXY_SESSION_LISTENFD(sess),
                    (struct sockaddr *)&PROXY_SESSION_LISTENSOCK(sess),
                    &PROXY_SESSION_SLEN(sess))) {
        PrintOutError("getsockname failed.\n");
        CleanupProxySession(sess);
        return NULL;
    }

    // TODO: consider replacing this with getifaddrs - that might cause issues
    // though where there is a routing mechanism in place between the eslogin
    // and real login. If we use the hostname, then the routing mechanism should
    // work.
    if (gethostname(lhost, MAXHOSTNAMELEN - 1)) {
        PrintOutError("gethostname failed.\n");
        CleanupProxySession(sess);
        return NULL;
    }

    // create the port string
    Asprintf(&lport, "%d", ntohs(PROXY_SESSION_LISTENSOCK(sess).sin_port));

    // create the temporary batch script from the provided template for WLM only
    switch (manager) {
        case WLM_none: {
            DPRINT("Starting proxy daemon using SSH\n");

            if (ssh_start_proxy(hostname, username, lhost, lport)) {
                Free(lport);
                CleanupProxySession(sess);
                return NULL;
            }

            break;
        }

        case WLM_qsub:
        case WLM_sbatch: {
            char *guard_proxy_loc;
            char **proxy_args;
            char *proxy_arg_str;
            char *popen_cmd;
            FILE *jobStream;

            // Find the guard_proxy binary in path, we will launch this directly
            if ((guard_proxy_loc = pathFind("guard_proxy", NULL)) == NULL) {
                PrintOutError("ERROR: Could not locate guard_proxy in PATH.\n");
                Free(lport);
                CleanupProxySession(sess);
                return NULL;
            }

            proxy_args =
                NewArgv(guard_proxy_loc, "-h", lhost, "-p", lport, NULL);
            // stringify the proxy_args array
            proxy_arg_str = ArgsToBuf(proxy_args, 5);
            // Cleanup
            Free(guard_proxy_loc);
            FreeArgv(proxy_args);

            if ((PROXY_SESSION_JOBFILE(sess) =
                     parseBatchScript(batch_template, proxy_arg_str)) == NULL) {
                Free(lport);
                Free(proxy_arg_str);
                CleanupProxySession(sess);
                return NULL;
            }

            PrintOutMsg("Submitting job to the batch system.\n");

            // cleanup
            Free(proxy_arg_str);

            // create the command string based on the workload manager
            switch (manager) {
                case WLM_none:
                    PrintOutError(
                        "Interal error: Invalid WLM detected. Impossible "
                        "condition realized!!!\n");
                    Free(lport);
                    CleanupProxySession(sess);
                    return NULL;

                case WLM_qsub:
                    Asprintf(&popen_cmd, "qsub %s",
                             PROXY_SESSION_JOBFILE(sess));
                    break;

                case WLM_sbatch:
                    Asprintf(&popen_cmd, "sbatch %s",
                             PROXY_SESSION_JOBFILE(sess));
                    break;
            }

            // launch the job
            if ((jobStream = popen(popen_cmd, "r")) == NULL) {
                PrintOutError("Failed to run job launch command.\n");
                CleanupProxySession(sess);
                Free(lport);
                return NULL;
            }

            // cleanup
            Free(popen_cmd);

            // TODO: Add code to read in the job identifier from the output
            // stream

            // Call pclose, ensure the exit status is 0, otherwise an error
            // occured
            /*
            if ((rc = pclose(jobStream)) != 0)
            {
                    PrintOutError("qsub returned with exit status %d.\n", rc);
                    CleanupProxySession(sess);
                    return NULL;
            }
            */

            DPRINT("Job launched.\n");

            break;
        }
    }

    // cleanup
    Free(lport);

    // accept on the listener socket for command stream
    if ((PROXY_SESSION_CMDFD(sess) =
             accept(PROXY_SESSION_LISTENFD(sess),
                    (struct sockaddr *)&PROXY_SESSION_CMDFD(sess),
                    &PROXY_SESSION_SLEN(sess))) == -1) {
        PrintOutError("Unable to accept incoming cmd stream connection.\n");
        CleanupProxySession(sess);
        return NULL;
    }

    // Set the command socket to TCP_NODELAY
    sopt = 1;
    if (setsockopt(PROXY_SESSION_CMDFD(sess), IPPROTO_TCP, TCP_NODELAY, &sopt,
                   sizeof(sopt)) < 0) {
        // Non-fatal error
        perror("setsockopt(TCP_NODELAY)");
    }

    DPRINT("Command stream attached.\n");

    // accept on the listener socket for error stream
    if ((PROXY_SESSION_ERRORFD(sess) =
             accept(PROXY_SESSION_LISTENFD(sess),
                    (struct sockaddr *)&PROXY_SESSION_ERRORSOCK(sess),
                    &PROXY_SESSION_SLEN(sess))) == -1) {
        PrintOutError("Unable to accept incoming error stream connection.\n");
        CleanupProxySession(sess);
        return NULL;
    }

    DPRINT("Error stream attached.\n");

    // Setup the error stream

    // Set the fd to be non-blocking
    if (fcntl(PROXY_SESSION_ERRORFD(sess), F_SETFL, O_NONBLOCK) < 0) {
        PrintOutError("Failed to set error fd to O_NONBLOCK.\n");
        CleanupProxySession(sess);
        return NULL;
    }

    // Create a file stream for the error stream. This is because we want to use
    // stdout functions directly on the socket
    if ((PROXY_SESSION_ERRORSTREAM(sess) =
             fdopen(PROXY_SESSION_ERRORFD(sess), "r")) == NULL) {
        PrintOutError("Error stream fdopen failed.\n");
        CleanupProxySession(sess);
        return NULL;
    }

    // accept on the listener socket for message stream
    if ((PROXY_SESSION_MSGFD(sess) =
             accept(PROXY_SESSION_LISTENFD(sess),
                    (struct sockaddr *)&PROXY_SESSION_MSGSOCK(sess),
                    &PROXY_SESSION_SLEN(sess))) == -1) {
        PrintOutError("Unable to accept incoming message stream connection.\n");
        CleanupProxySession(sess);
        return NULL;
    }

    DPRINT("Msg stream attached.\n");

    // Setup the error stream

    // Set the fd to be non-blocking
    if (fcntl(PROXY_SESSION_MSGFD(sess), F_SETFL, O_NONBLOCK) < 0) {
        PrintOutError("Failed to set message fd to O_NONBLOCK.\n");
        CleanupProxySession(sess);
        return NULL;
    }

    // Create a file stream for the message stream. This is because we want to
    // use stdout functions directly on the socket
    if ((PROXY_SESSION_MSGSTREAM(sess) =
             fdopen(PROXY_SESSION_MSGFD(sess), "r")) == NULL) {
        PrintOutError("Message stream fdopen failed.\n");
        CleanupProxySession(sess);
        return NULL;
    }

    DPRINT("Session started.\n");

    // Create a proto_session object
    rtn = (proto_session *)Alloc(sizeof(proto_session));
    rtn->sess_proto = proto;
    rtn->sess_obj = sess;

    return rtn;
}

/*
** Mark an existing proxy session as valid
*/
static void DbgPROXYSaveSession(proto_session *session) {
    proxy_session *sess;
    proxy_msg_header msg_head;

    if ((session == NULL) ||
        ((sess = (proxy_session *)session->sess_obj) == NULL)) {
        PrintOutError("Internal error: Attempted to save null session.\n");
        return;
    }

    // Set this session as valid
    PROXY_SESSION_VALID(sess) = 1;

    // Send a command to the proxy daemon to keep the session alive
    msg_head.msg_id =
        -1;  // No id since this command is for the session, not an instance
    msg_head.msg_len =
        0;  // No length, not actually forwarding a protocol command here
    msg_head.msg_type = MSG_SAVE_SESSION;

    if (send(PROXY_SESSION_CMDFD(sess), &msg_head, sizeof(proxy_msg_header),
             0) != sizeof(proxy_msg_header)) {
        PrintOutError("Could not send header on cmd stream.\n");
        PrintOutError("Failed to save session.\n");
        PROXY_SESSION_VALID(sess) = 0;
        return;
    }

    // Block until we recv the header, we expect an ok back at which point
    // we will cleanup
    if (recv(PROXY_SESSION_CMDFD(sess), &msg_head, sizeof(proxy_msg_header),
             MSG_WAITALL) < 0) {
        PrintOutError("Failed to recv header on event socket.\n");
        PrintOutError("Failed to save session.\n");
        PROXY_SESSION_VALID(sess) = 0;
        return;
    }

    // ensure we got an okay
    if (msg_head.msg_type != MSG_OK) {
        PrintOutError("Failed to receive ok message during session save.\n");
        PrintOutError("Failed to save session.\n");
        PROXY_SESSION_VALID(sess) = 0;
    }
}

/*
** Cleanup an existing session, making sure it is no longer valid
*/
static void DbgPROXYCleanupSession(proto_session *session) {
    proxy_session *sess;
    proxy_msg_header msg_head;

    if ((session == NULL) ||
        ((sess = (proxy_session *)session->sess_obj) == NULL)) {
        PrintOutError("Internal error: Attempted to save null session.\n");
        return;
    }

    // Set this session as invalid
    PROXY_SESSION_VALID(sess) = 0;

    // Send a command to the proxy daemon to keep the session alive
    msg_head.msg_id =
        -1;  // No id since this command is for the session, not an instance
    msg_head.msg_len =
        0;  // No length, not actually forwarding a protocol command here
    msg_head.msg_type = MSG_CLEANUP_SESSION;

    if (send(PROXY_SESSION_CMDFD(sess), &msg_head, sizeof(proxy_msg_header),
             0) != sizeof(proxy_msg_header)) {
        PrintOutError("Could not send header on cmd stream.\n");
    }

    // Block until we recv the header, we expect an ok back at which point
    // we will cleanup
    if (recv(PROXY_SESSION_CMDFD(sess), &msg_head, sizeof(proxy_msg_header),
             MSG_WAITALL) < 0) {
        PrintOutError("Failed to recv header on event socket.\n");
    }

    // ensure we got an okay
    if (msg_head.msg_type != MSG_OK) {
        PrintOutError("Failed to receive ok message during session cleanup.\n");
    }

    // Call the cleanup function to ensure it gets cleaned up if there are no
    // longer any valid instances connected with the session
    CleanupProxySession(sess);

    // Free the proto_session object
    Free(session);
}

/*
** Attach to user program through proxy daemon
*/
static int DbgPROXYAttach(dbginvoke_t *di, proto_session *session,
                          const char *app_ident, const char *arch,
                          const char *backend, int gpu_flag, int debug_flag) {
    proxy_session *sess;
    proxy_type *net;
    proxy_msg_header msg_head;
    char *msg;
    int *ev_fd_ptr;
    int *err_fd_ptr;
    int *msg_fd_ptr;

    // Ensure the dbginvoke_t object was passed in
    if (di == NULL) return 1;

    // Set the initial protocol event callback to use our custom event handler
    // for launching
    di->d_proto_ev_callback = cbPROXYAttach;

    // Make sure the user passed in a session
    if ((session == NULL) ||
        ((sess = (proxy_session *)session->sess_obj) == NULL)) {
        PrintOutError("No valid session provided to attach!\n");
        return 1;
    }

    // if PERFLOG is turned on, we need to create a string to the location to
    // create the log file.
#ifdef PERFLOG
    char *envDir;
    char logfile[PATH_MAX];

    if ((envDir = getenv(DBG_LOG_ENV_VAR)) == (char *)NULL) {
        // user didn't set the environment variable, so lets
        // read the HOME variable
        if ((envDir = getenv("HOME")) == (char *)NULL) {
            // couldn't read HOME, so lets default to tmp
            envDir = "/tmp";
        }
    }

    // create the fullpath string to the log file using PATH_MAX plus a null
    // term
    snprintf(logfile, PATH_MAX + 1, "%s/proxy_perf_frontend.log", envDir);

    DPERFLOG(CreateTimerLog(logfile));
    DPERFLOG(SetLogInfo("Attaching to processes\n"));
    DPERFLOG(SetTimerStart("Attach total"));
#endif /* PERFLOG */

    // Create a new proxy_type object
    net = NewProxyType(sess);

    // setup the header
    msg_head.msg_id = PROXY_ID(net);
    msg_head.msg_type = MSG_ATTACH;

    // Create the attach args string
    msg = AttachArgsToSocProto(app_ident, arch, backend, gpu_flag, debug_flag);

    // set the length in the header
    msg_head.msg_len = (int)strlen(msg) * sizeof(char);

    // Send the header
    if (send(PROXY_CMDFD(net), &msg_head, sizeof(proxy_msg_header), 0) !=
        sizeof(proxy_msg_header)) {
        PrintOutError("Could not send header on cmd stream.\n");
        FreeProxyType(net);
        return 1;
    }

    // Send the msg
    if (send(PROXY_CMDFD(net), msg, msg_head.msg_len, 0) != msg_head.msg_len) {
        PrintOutError("Could not send invoke args on cmd stream.\n");
        FreeProxyType(net);
        return 1;
    }

    // Done with msg
    Free(msg);

    DPRINT("DbgPROXYAttach: Waiting for proxy connection...\n");

    // accept on the listener socket for event stream
    if ((PROXY_EVENTFD(net) = accept(PROXY_LISTENFD(net),
                                     (struct sockaddr *)&PROXY_EVENTSOCK(net),
                                     &PROXY_SLEN(net))) == -1) {
        PrintOutError("Unable to accept incoming event stream connection.\n");
        FreeProxyType(net);
        return 1;
    }

    DPRINT("DbgPROXYAttach: Event stream attached.\n");

    // accept on the listener socket for output stream
    if ((PROXY_OUTPUTFD(net) = accept(PROXY_LISTENFD(net),
                                      (struct sockaddr *)&PROXY_OUTPUTSOCK(net),
                                      &PROXY_SLEN(net))) == -1) {
        PrintOutError("Unable to accept incoming output stream connection.\n");
        FreeProxyType(net);
        return 1;
    }

    DPRINT("DbgPROXYAttach: Output stream attached.\n");

    // Setup the output stream

    // Create a file stream for the output stream. This is because we want to
    // use stdout functions directly on the socket
    if ((PROXY_OUTPUTSTREAM(net) = fdopen(PROXY_OUTPUTFD(net), "r")) == NULL) {
        PrintOutError("Output stream fdopen failed.\n");
        FreeProxyType(net);
        return 1;
    }

    // Now set the stream for program output.
    // TODO: If we really wanted to, we could attach to the existing aprun
    // process here and redirect its /proc stdout and stderr to our output
    // stream handler.

    DPRINT("DbgPROXYAttach: Proxy connected.\n");

    // Note that the bitset's will be created in the callback. This is different
    // in this protocol implementation since we don't have access to alps
    // information on this end. We blindly accept what the proxy daemon tells
    // us. The d_nproc is also set there.

    // Set the network object in the dbginvoke
    di->d_proto_net = net;

    // create the d_evfds list for the di
    di->d_evfds = NewList();

    // create an int to hold the event stream fd.
    ev_fd_ptr = (int *)Alloc(sizeof(int));
    *ev_fd_ptr = PROXY_EVENTFD(net);

    // Add the event_stream fd to the d_evfds list
    // This allows DbgCheckEvents to select on protocol events
    AddToList(di->d_evfds, (void *)ev_fd_ptr);

    // create an int to hold the error stream fd.
    err_fd_ptr = (int *)Alloc(sizeof(int));
    *err_fd_ptr = PROXY_SESSION_ERRORFD(sess);

    // Add the error stream fd to the d_evfds list
    // This allows DbgCheckEvents to select on error events
    AddToList(di->d_evfds, (void *)err_fd_ptr);

    // create an int to hold the message stream fd.
    msg_fd_ptr = (int *)Alloc(sizeof(int));
    *msg_fd_ptr = PROXY_SESSION_MSGFD(sess);

    // Add the message stream fd to the d_evfds list
    // This allows DbgCheckEvents to select on message events
    AddToList(di->d_evfds, (void *)msg_fd_ptr);

    PrintOutMsg("Waiting to attach to application, please wait...\n");

    DPERFLOG(SetTimerStopAndLog("Attach total"));

    // Here we want to wait for the dbginvoke to start as long as the callback
    // is still valid
    callback_failure = 0;
    while (di->d_started == 0 && callback_failure == 0) {
        if (WaitForProtoStartupEvent(di) == -1 || callback_failure == 1) {
            PrintOutError("Could not receive attach callbacks!\n");
            return 1;
        }
    }

    // All done. Everything is ready to go at this point.
    return 0;
}

/*
** Compatible callback routine for attaching with the proxy
*/
static int cbPROXYAttach(dbginvoke_t *di, proto_msg_set *set) {
    proto_message *msg;
    dbgevent_t *e = NULL;
    int err = 0;

    if (set == NULL) return 0;

    ResetProtoMsgSet(set);
    while ((msg = NextProtoMsgInSet(set)) != NULL) {
        // convert this protocol msg string to an event
        e = SocProtoToEvent(msg->message);

        // Note that the proxy will artificially create the DBG_OK message for
        // every rank in the application. Since we do not know how many ranks
        // exist until after the attach completes on this end, we just blindly
        // accept what the proxy daemon tells us
        if (di->d_bitset_full == NULL) {
            di->d_nproc = bitset_size(msg->group);

            di->d_bitset_full = bitset_new(bitset_size(msg->group));
            bitset_invert(di->d_bitset_full);

            // Create the empty launched bitset
            di->d_bitset_launched = bitset_new(bitset_size(msg->group));
        }

        bitset_oreq(di->d_bitset_launched, msg->group);

        if (e->ev_event != DBGEV_OK) {
            err = 1;
            PrintOutError("Received unknown event %s.\n",
                          DbgevToStr(e->ev_event));
        }

        // Done with the event, free it
        FreeEvent(e);
    }

    if (err) return 1;

    DPRINT("cbPROXYAttach received responses [%s]!\n",
           bitset_to_str(di->d_bitset_launched));

    // Check if we have received a message from every rank
    if (bitset_eq(di->d_bitset_launched, di->d_bitset_full)) {
        // Reset the protocol event callback to the dbgclient default
        di->d_proto_ev_callback = DbgDispatchProtoEvents;

        // Set the started bit in the di
        di->d_started = 1;

        DPERFLOG(SetTimerStopAndLog("stream_init"));

        DPRINT("proxy callback complete.\n");
    }

    return 0;
}

/*
** Invoke user program through proxy daemon
*/
static int DbgPROXYLaunch(dbginvoke_t *di, proto_session *session,
                          const char *prog, char **argv, const char *arch,
                          char **arch_args, const char *arch_input,
                          const char *backend, const char* gdb,  char **env, const char *cwd,
                          int gpu_flag, int debug_flag, int early_entry_flag,
                          bool isNonStop) {
    proxy_session *sess;
    proxy_type *net;
    proxy_msg_header msg_head;
    char *msg;
    int *ev_fd_ptr;
    int *err_fd_ptr;
    int *msg_fd_ptr;
    int *outp_fd_ptr;

    // Ensure the dbginvoke_t object was passed in
    if (di == NULL) return 1;

    // Set the initial protocol event callback to use our custom event handler
    // for launching
    di->d_proto_ev_callback = cbPROXYInvoke;

    // Make sure the user passed in a session
    if ((session == NULL) ||
        ((sess = (proxy_session *)session->sess_obj) == NULL)) {
        PrintOutError("No valid session provided to launch!\n");
        return 1;
    }

    // if PERFLOG is turned on, we need to create a string to the location to
    // create the log file.
#ifdef PERFLOG
    char *envDir;
    char logfile[PATH_MAX];

    if ((envDir = getenv(DBG_LOG_ENV_VAR)) == (char *)NULL) {
        // user didn't set the environment variable, so lets
        // read the HOME variable
        if ((envDir = getenv("HOME")) == (char *)NULL) {
            // couldn't read HOME, so lets default to tmp
            envDir = "/tmp";
        }
    }

    // create the fullpath string to the log file using PATH_MAX plus a null
    // term
    snprintf(logfile, PATH_MAX + 1, "%s/proxy_perf_frontend.log", envDir);

    DPERFLOG(CreateTimerLog(logfile));
    DPERFLOG(SetTimerStart("Invoke total"));
#endif /* PERFLOG */

    // Create a new proxy_type object
    net = NewProxyType(sess);

    // setup the header
    msg_head.msg_id = PROXY_ID(net);
    msg_head.msg_type = MSG_INVOKE;

    // Create the invoke args string
    msg = LaunchArgsToSocProto(prog, argv, di->d_nproc, arch, arch_args,
                               arch_input, backend, gdb, env, cwd, gpu_flag,
                               debug_flag, early_entry_flag, isNonStop);

    // set the length in the header
    msg_head.msg_len = (int)strlen(msg) * sizeof(char);

    // Send the header
    if (send(PROXY_CMDFD(net), &msg_head, sizeof(proxy_msg_header), 0) !=
        sizeof(proxy_msg_header)) {
        PrintOutError("Could not send header on cmd stream.\n");
        FreeProxyType(net);
        return 1;
    }

    // Send the msg
    if (send(PROXY_CMDFD(net), msg, msg_head.msg_len, 0) != msg_head.msg_len) {
        PrintOutError("Could not send invoke args on cmd stream.\n");
        FreeProxyType(net);
        return 1;
    }

    // Done with msg
    Free(msg);

    DPRINT("Waiting for proxy connection...\n");

    // accept on the listener socket for event stream
    if ((PROXY_EVENTFD(net) = accept(PROXY_LISTENFD(net),
                                     (struct sockaddr *)&PROXY_EVENTSOCK(net),
                                     &PROXY_SLEN(net))) == -1) {
        PrintOutError("Unable to accept incoming event stream connection.\n");
        FreeProxyType(net);
        return 1;
    }

    DPRINT("DbgPROXYInvoke: Event stream attached.\n");

    // accept on the listener socket for output stream
    if ((PROXY_OUTPUTFD(net) = accept(PROXY_LISTENFD(net),
                                      (struct sockaddr *)&PROXY_OUTPUTSOCK(net),
                                      &PROXY_SLEN(net))) == -1) {
        PrintOutError("Unable to accept incoming output stream connection.\n");
        FreeProxyType(net);
        return 1;
    }

    DPRINT("DbgPROXYInvoke: Output stream attached.\n");

    // Setup the output stream

    // Create a file stream for the output stream. This is because we want to
    // use stdout functions directly on the socket
    if ((PROXY_OUTPUTSTREAM(net) = fdopen(PROXY_OUTPUTFD(net), "r")) == NULL) {
        PrintOutError("Output stream fdopen failed.\n");
        FreeProxyType(net);
        return 1;
    }

    // Set the stream to non-blocking
    if (fcntl(fileno(PROXY_OUTPUTSTREAM(net)), F_SETFL, O_NONBLOCK) < 0) {
        PrintOutError("pipe: Could not fcntl to O_NONBLOCK.\n");
    }

    // Set the protocol program output stream callback
    di->d_proto_outp_callback = DbgDispatchProtoOutput;

    DPRINT("Proxy connected.\n");

    // Create the full bitset based on the number of ranks
    di->d_bitset_full = bitset_new(di->d_nproc);
    bitset_invert(di->d_bitset_full);

    // Create the empty launched bitset
    di->d_bitset_launched = bitset_new(di->d_nproc);

    // Set the network object in the dbginvoke
    di->d_proto_net = net;

    // create the d_evfds list for the di
    di->d_evfds = NewList();

    // create an int to hold the event stream fd.
    ev_fd_ptr = (int *)Alloc(sizeof(int));
    *ev_fd_ptr = PROXY_EVENTFD(net);

    // Add the event_stream fd to the d_evfds list
    // This allows DbgCheckEvents to select on protocol events
    AddToList(di->d_evfds, (void *)ev_fd_ptr);

    // create an int to hold the error stream fd.
    err_fd_ptr = (int *)Alloc(sizeof(int));
    *err_fd_ptr = PROXY_SESSION_ERRORFD(sess);

    // Add the error stream fd to the d_evfds list
    // This allows DbgCheckEvents to select on error events
    AddToList(di->d_evfds, (void *)err_fd_ptr);

    // create an int to hold the message stream fd.
    msg_fd_ptr = (int *)Alloc(sizeof(int));
    *msg_fd_ptr = PROXY_SESSION_MSGFD(sess);

    // Add the message stream fd to the d_evfds list
    // This allows DbgCheckEvents to select on message events
    AddToList(di->d_evfds, (void *)msg_fd_ptr);

    // create an int to hold the output stream fd.
    outp_fd_ptr = (int *)Alloc(sizeof(int));
    *outp_fd_ptr = PROXY_OUTPUTFD(net);

    // Add the output stream fd to the d_evfds list
    // This allows DbgCheckEvents to select on output events
    AddToList(di->d_evfds, (void *)outp_fd_ptr);

    PrintOutMsg("Waiting for job to start, please wait...\n");

    DPERFLOG(SetTimerStopAndLog("Invoke total"));

    // Here we want to wait for the dbginvoke to start as long as the callback
    // is still valid
    callback_failure = 0;
    while (di->d_started == 0 && callback_failure == 0) {
        if (WaitForProtoStartupEvent(di) == -1 || callback_failure == 1) {
            PrintOutError("Could not receive launch callbacks!\n");
            return 1;
        }
    }

    // All done. Everything is ready to go at this point.
    return 0;
}

/*
** Compatible callback routine for invoking with the proxy
*/
static int cbPROXYInvoke(dbginvoke_t *di, proto_msg_set *set) {
    proto_message *msg;
    dbgevent_t *e = NULL;
    int err = 0;

    if (set == NULL) return 0;

    ResetProtoMsgSet(set);
    while ((msg = NextProtoMsgInSet(set)) != NULL) {
        // convert this protocol msg string to an event
        e = SocProtoToEvent(msg->message);

        bitset_oreq(di->d_bitset_launched, msg->group);

        if (e->ev_event != DBGEV_OK) {
            err = 1;
            PrintOutError("Received unknown event %s.\n",
                          DbgevToStr(e->ev_event));
        }

        // Done with the event, free it
        FreeEvent(e);
    }

    if (err) return 1;

    DPRINT("cbPROXYInvoke received responses [%s]!\n",
           bitset_to_str(di->d_bitset_launched));

    // Check if we have received a message from every rank
    if (bitset_eq(di->d_bitset_launched, di->d_bitset_full)) {
        // Reset the protocol event callback to the dbgclient default
        di->d_proto_ev_callback = DbgDispatchProtoEvents;

        // Set the started bit in the di
        di->d_started = 1;

        DPERFLOG(SetTimerStopAndLog("stream_init"));

        DPRINT("proxy callback complete.\n");
    }

    return 0;
}

/*
**********************************************************
**********************************************************
************ WRAPPERS FOR PROXY ROUTINES *****************
**********************************************************
**********************************************************
*/

// Helper function to receive a proto message
static proto_msg_set *DbgPROXYRecvProtoMsg(int net_fd) {
    proxy_msg_header soc_head;
    char *soc_buf;
    proto_message *msg;
    proto_msg_set *res;

    // First recv the header
    if (recv(net_fd, &soc_head, sizeof(proxy_msg_header), MSG_WAITALL) <= 0) {
        PrintOutError("Failed to recv header on event socket.\n");
        return NULL;
    }

    // Now create a string buffer to receive the message
    soc_buf = (char *)Alloc(soc_head.msg_len);

    // recv the message
    if (recv(net_fd, soc_buf, soc_head.msg_len, MSG_WAITALL) <= 0) {
        PrintOutError("Failed to recv msg on event socket.\n");
        Free(soc_buf);
        return NULL;
    }

    // convert this to a proto_msg *
    if ((msg = SocProtoToProtoMsg(soc_buf)) == NULL) {
        PrintOutError("Could not convert event stream msg to proto msg.\n");
        Free(soc_buf);
        return NULL;
    }

    // cleanup
    Free(soc_buf);

    // Create the proto msg set
    res = NewProtoMsgSet();
    AddProtoMsgSet(res, msg);

    ResetProtoMsgSet(res);

    return res;
}

static void DbgPROXYDispatchEvents(dbginvoke_t *di, int ev_fd) {
    proxy_type *network = (proxy_type *)di->d_proto_net;
    proto_msg_set *ev_res;
    char *line;

    // fix segfault
    if (ev_fd == -1 || di->d_proto_net == NULL) {
        // Something is seriously wrong, force quit
        di->d_isShutdown = 1;
        ForceQuit();
        return;
    }

    // dispatch events
    if (ev_fd == PROXY_EVENTFD(network)) {
        // get the message
        if ((ev_res = DbgPROXYRecvProtoMsg(PROXY_EVENTFD(network))) == NULL) {
            // If this happens, it means the proxy daemon died. We should force
            // quit to happen here since it is unexpected. We expect that the
            // proxy already sent us an error string, so print a brief one.
            di->d_isShutdown = 1;
            ForceQuit();
            return;
        }

        // Call the protocol specific callback function to handle this event msg
        (*(di->d_proto_ev_callback))(di, ev_res);

        // Free the message set
        FreeProtoMsgSet(ev_res);

        return;
    }

    // dispatch output
    if (ev_fd == PROXY_OUTPUTFD(network)) {
        // Call the protocol specific callback function to handle the output
        (*(di->d_proto_outp_callback))(di, PROXY_OUTPUTSTREAM(network));

        return;
    }

    // dispatch errors
    if (ev_fd == PROXY_SESSION_ERRORFD(PROXY_SESSION(network))) {
        line = (char *)Alloc(BUFSIZ);
        memset(line, 0, BUFSIZ);

        while (fgets(line, BUFSIZ,
                     PROXY_SESSION_ERRORSTREAM(PROXY_SESSION(network))) > 0) {
            PrintOutError("%s", line);
            memset(line, 0, BUFSIZ);
        }

        Free(line);

        // FIXME: This is a major hack...
        // If we get an error message before startup finishes, then we assume
        // that startup will never complete. This may or may not be true, but as
        // the interface stands right now things will have to be changed around
        // to handle errors properly. The issue is that we send the command off
        // to the proxy daemon and wait for it's callback to come in. That
        // callback consists of proto messages for each rank, and we might not
        // know how many ranks exist as is the case for the attach scenario. I
        // don't know how to make this work, this hack is the only thing I can
        // come up with right now.
        if (di->d_started == 0) {
            callback_failure = 1;
        }

        return;
    }

    // dispatch messages
    if (ev_fd == PROXY_SESSION_MSGFD(PROXY_SESSION(network))) {
        line = (char *)Alloc(BUFSIZ);
        memset(line, 0, BUFSIZ);

        while (fgets(line, BUFSIZ,
                     PROXY_SESSION_MSGSTREAM(PROXY_SESSION(network))) > 0) {
            PrintOutMsg("%s", line);
            memset(line, 0, BUFSIZ);
        }

        Free(line);

        return;
    }
}

static bitset *DbgPROXYSendCmd(dbginvoke_t *di, bitset *target,
                               const char *request) {
    proxy_type *network = (proxy_type *)di->d_proto_net;
    bitset *received;
    proto_msg_set *pmsg_set;
    proto_message *pmsg;
    char *msg;
    proxy_msg_header msg_head;

    if (request == NULL) return NULL;

    // send command only if not already in a shutdown state
    if (di->d_isShutdown) {
        return NULL;
    }

    // Convert the arguments to a proto message
    pmsg = NewProtoMsg(bitset_dup(target), strdup(request));
    // Convert this to a string
    msg = ProtoMsgToSocProto(pmsg);
    FreeProtoMsg(pmsg);

    // Send the header
    msg_head.msg_id = PROXY_ID(network);
    msg_head.msg_len = (int)strlen(msg) * sizeof(char);
    msg_head.msg_type = MSG_SEND_CMD;

    if (send(PROXY_CMDFD(network), &msg_head, sizeof(proxy_msg_header), 0) !=
        sizeof(proxy_msg_header)) {
        PrintOutError("Could not send header on cmd stream.\n");
        return NULL;
    }

    // Send the msg
    if (send(PROXY_CMDFD(network), msg, msg_head.msg_len, 0) !=
        msg_head.msg_len) {
        PrintOutError("Could not send msg on cmd stream.\n");
        return NULL;
    }

    // Done with msg
    Free(msg);

    // recv the receiver set
    if ((pmsg_set = DbgPROXYRecvProtoMsg(PROXY_CMDFD(network))) == NULL) {
        PrintOutError("Could not recv reply from proxy.\n");
        return NULL;
    }

    ResetProtoMsgSet(pmsg_set);
    if ((pmsg = NextProtoMsgInSet(pmsg_set)) == NULL) {
        PrintOutError("Got empty proto set from proxy.\n");
        return NULL;
    }

    received = bitset_dup(pmsg->group);

    // cleanup
    FreeProtoMsgSet(pmsg_set);

    DPRINT("INFO: Sending cmd to network[%p]:set[%s]\n", network,
           bitset_to_set(target));

    return received;
}

// Here received means the bitset that represents the ranks we sent the initial
// command to. Change this to something that makes more sense.
static proto_msg_set *DbgPROXYRecvReply(dbginvoke_t *di, bitset *target,
                                        bitset *received) {
    proxy_type *network = (proxy_type *)di->d_proto_net;
    int full = 0;
    proto_msg_set *pmsg_set = NULL;
    proto_message *pmsg;
    proto_msg_set *result = NULL;
    proxy_msg_header msg_head;
    char *msg;

    // recv reply only if not already in a shutdown state
    if (di->d_isShutdown) {
        return NULL;
    }

    // Convert the received bitset to a recv type message
    msg = RecvToSocProto(target, received);

    // send a recv command by sending the header with a recv type
    msg_head.msg_id = PROXY_ID(network);
    msg_head.msg_len = (int)strlen(msg) * sizeof(char);
    msg_head.msg_type = MSG_RECV_REPLY;

    if (send(PROXY_CMDFD(network), &msg_head, sizeof(proxy_msg_header), 0) !=
        sizeof(proxy_msg_header)) {
        PrintOutError("Could not send header on cmd stream.\n");
        return NULL;
    }

    // send the msg
    if (send(PROXY_CMDFD(network), msg, msg_head.msg_len, 0) !=
        msg_head.msg_len) {
        PrintOutError("Could not send msg on cmd stream.\n");
        return NULL;
    }

    // cleanup
    Free(msg);

    while (!full) {
        if ((pmsg_set = DbgPROXYRecvProtoMsg(PROXY_CMDFD(network))) == NULL) {
            PrintOutError("Could not recv reply from proxy.\n");
            ResetProtoMsgSet(result);
            return result;
        }

        // If this is the first message set, then use this as our return type,
        // otherwise merge the set with our return type.
        if (result == NULL) {
            result = pmsg_set;
            // process the pmsg_set to get the received set
            while ((pmsg = NextProtoMsgInSet(pmsg_set)) != NULL) {
                // add this group to the received group
                bitset_oreq(received, pmsg->group);
            }
        } else {
            // add each message to the result
            while ((pmsg = RemoveProtoMsgInSet(pmsg_set)) != NULL) {
                // add this group to the received group
                bitset_oreq(received, pmsg->group);
                AddProtoMsgSet(result, pmsg);
            }
        }

        DPRINT("INFO: Recieved replies from set[%s]\n",
               bitset_to_str(received));

        // Check to see if we have received all responses
        full = bitset_eq(target, received);
    }

    ResetProtoMsgSet(result);

    return result;
}

/*
** DBG_EXIT: Tell server to quit immediately.
** If detach is true, then we detach from application instead of killing it.
*/
static int DbgPROXYExit(dbginvoke_t *di, int detach) {
    proxy_type *network = (proxy_type *)di->d_proto_net;
    proxy_msg_header msg_head;
    char *msg;

    // send exit only if not already in a shutdown state
    if (!di->d_isShutdown) {
        // Create a message containing the args
        msg = ExitArgsToSocProto(detach);

        // send a recv command by sending the header with a recv type
        msg_head.msg_id = PROXY_ID(network);
        msg_head.msg_len = (int)strlen(msg) * sizeof(char);
        msg_head.msg_type = MSG_EXIT;

        if (send(PROXY_CMDFD(network), &msg_head, sizeof(proxy_msg_header),
                 0) != sizeof(proxy_msg_header)) {
            PrintOutError("Could not send header on cmd stream.\n");
        }

        // send the msg
        if (send(PROXY_CMDFD(network), msg, msg_head.msg_len, 0) !=
            msg_head.msg_len) {
            PrintOutError("Could not send msg on cmd stream.\n");
        }

        // Block until we recv the header, we expect an ok back at which point
        // we will cleanup
        if (recv(PROXY_CMDFD(network), &msg_head, sizeof(proxy_msg_header),
                 MSG_WAITALL) < 0) {
            PrintOutError("Failed to recv header on event socket.\n");
        }

        // ensure we got an okay
        if (msg_head.msg_type != MSG_OK) {
            // non-fatal error
            PrintOutError("Failed to receive ok message during exit.\n");
        }
    }

    // cleanup the proxy instance
    FreeProxyType(network);

    // clear the network pointer
    di->d_proto_net = NULL;

    return 0;
}

static void DbgPROXYDestroyClient(dbginvoke_t *di) {
    proxy_type *network = (proxy_type *)di->d_proto_net;

    // Check to see if the network type has been destroyed
    if (network != NULL) {
        // call the exit function to force the application to quit
        DbgPROXYExit(di, 0);
    }
}
