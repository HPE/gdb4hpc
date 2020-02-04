/******************************************************************************\
 * cli_socket.c - External socket CLI routines.
 *
 * © 2013 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 ******************************************************************************/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <stddef.h>
#include <fcntl.h>
#include <memory.h>
#include <signal.h>
#include <netdb.h>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

#include "client.h"
#include "useful.h"

#define LINEBUF_BLOCK_SIZE	1024
#define RECONNECT_TRIES		10

/*
 * Internal prototypes
 */
static int	cli_socket_init(cli_stream_t **);
static int	cli_socket_destroy(cli_stream_t **);
static int	cli_socket_handle_input(cli_stream_t ** stream);

cli_externals_t *
cli_get_socket_funcs(void)
{
	cli_externals_t *rtn;
	
	rtn = (cli_externals_t *)Alloc(sizeof(cli_externals_t));
	
	rtn->external_init =			cli_socket_init;
	rtn->external_destroy =			cli_socket_destroy;
	rtn->external_handle_input =	cli_socket_handle_input;
	rtn->external_handle_error =	NULL;	// We don't handle debugger errors for this stream
	rtn->external_save_stream =		NULL;
	rtn->external_restore_stream =	NULL;
	rtn->external_reset_stream =	NULL;
	rtn->external_clear_line =		NULL;
	
	return rtn;
}

static int
cli_socket_init(cli_stream_t ** stream)
{
	cli_stream_t *		cs = (*stream);
	int 				s_fd = -1;
	int					es_fd = -1;
	FILE *				w_fp = NULL;
	FILE *				e_fp = NULL;
	// setup socket connection and redirect stdout/stderr to socket
	int 				opt, rc;
	struct addrinfo		hints;	// hints object for call to getaddrinfo
	struct addrinfo *	host;	// addrinfo object for hostname
	int					tries = 0;
	
	memset(&hints, 0, sizeof(hints));	// ensure the hints struct is cleared
	hints.ai_family = AF_UNSPEC;		// use AF_INET6 to force IPv6
	hints.ai_socktype = SOCK_STREAM;
	
	// get the addrinfo of the hostname
	if ((rc = getaddrinfo(cs->cs_socket_host, cs->cs_socket_port, &hints, &host)) != 0)
	{
		PrintOutError("Getaddrinfo failed: %s\n", gai_strerror(rc));
		return 1;
	}

	// Create the I/O socket
	if ((s_fd = socket( host->ai_family, host->ai_socktype, host->ai_protocol)) < 0)
	{
		perror("socket");
		return 1;
	}
	
	// Set the socket to TCP_NODELAY
	opt = 1;
	if (setsockopt(s_fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)) < 0) 
	{
		perror("setsockopt(TCP_NODELAY)");
	}
	
	// Connect the I/O socket to host
	while (connect(s_fd, host->ai_addr, host->ai_addrlen)) 
	{
		perror("connect");
		sleep(2);
		if (tries++ == RECONNECT_TRIES)
		{
			return 1;
		}
	}
	
	// reset counter
	tries = 0;
	
	// Cleanup the host structure
	freeaddrinfo(host);
	host = NULL;
	
	// get the addrinfo of the hostname for the error port
	if ((rc = getaddrinfo(cs->cs_socket_host, cs->cs_socket_error_port, &hints, &host)) != 0)
	{
		PrintOutError("Getaddrinfo failed: %s\n", gai_strerror(rc));
		return 1;
	}
	
	// Create the error socket
	if ((es_fd = socket( host->ai_family, host->ai_socktype, host->ai_protocol)) < 0)
	{
		perror("socket");
		return 1;
	}
	
	// Set the socket to TCP_NODELAY
	opt = 1;
	if (setsockopt(es_fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)) < 0) 
	{
		perror("setsockopt(TCP_NODELAY)");
	}
	
	// Connect the error socket to host
	while (connect(es_fd, host->ai_addr, host->ai_addrlen)) 
	{
		perror("connect");
		sleep(2);
		if (tries++ == RECONNECT_TRIES)
		{
			close(s_fd);
			return 1;
		}
	}
	
	// Cleanup the host structure
	freeaddrinfo(host);
	host = NULL;
	
	// Create file streams for the output and error sockets.
	w_fp = fdopen( s_fd, "w" );
	e_fp = fdopen( es_fd, "w" );
	
	if (w_fp == NULL || e_fp == NULL) 
	{
		PrintOutError("fdopen failed\n" );
		return 1;
	}
	
	// Set the stream members based on the args
	
	// The socket stream needs to do a recv on the socket instead of reading from
	// the stdin stream. The stdin stream is line buffered, meaning that if it
	// recieves more than an entire line, the select won't see the other stuff
	// and we will only process a single line.
	
	cs->cs_fpin = NULL;	// there is no input stream since we need to recv
	cs->cs_fpout = w_fp;
	cs->cs_fperr = e_fp;
	cs->cs_fdin = s_fd; // this points to the fd of the socket for recv
	cs->cs_fdout = s_fd;
	cs->cs_fderr = es_fd;
	
	// set the output stream to be line buffered.
	setvbuf(cs->cs_fperr, NULL, _IONBF, 0);
	
	// set the error stream to be unbuffered.
	setlinebuf(cs->cs_fpout);
	
	return 0;
}

static int
cli_socket_destroy(cli_stream_t ** stream)
{
	cli_stream_t * cs = (*stream);
	
	Free(cs->cs_ext_funcs);
	
	// close the stdin fd since this is the socket
	if (cs->cs_fdin != -1)
	{
		close(cs->cs_fdin);
	}
	
	// fclose stdout and stderr since those also point at sockets
	if (cs->cs_fpout != NULL)
	{
		fclose(cs->cs_fpout);
	}
	if (cs->cs_fperr != NULL)
	{
		fclose(cs->cs_fperr);
	}
	
	return 0;
}

static int
cli_socket_handle_input(cli_stream_t ** stream)
{
	int				n;
	ssize_t 		num;
	cli_stream_t *	cs;
	char *			cmd_buf;
	
	DPRINT("In cli_socket_handle_input\n");
	
	if (stream == NULL)
		goto cli_socket_handle_input_error;
		
	// set the cli stream
	cs = (*stream);

	// This prevents command output from saving or restoring the stream, since
	// we don't allow input to happen while processing a command.
	cs->cs_handling++;
	
	// ensure there is data to read on the input socket. If there isn't, that means
	// the socket was closed. In which case we should force things to quit since we lost
	// our communication mechanism.
	// TODO: Is this really needed??? I think recv should detect that the socket closed,
	// but empirical evidence says otherwise.
	if (ioctl(cs->cs_fdin, FIONREAD, &n) < 0)
	{
		PrintOutError("ioctl failed.\n");
		Client.quit = 1;
		goto cli_socket_handle_input_error;
	}
	if (n < 1)
	{
		Client.quit = 1;
		goto cli_socket_handle_input_error;
	}
	
	// Allocate a new buffer or else ensure there is enough room to read another
	// char plus a null term. Note that cs_buf_point points at an empty location
	// to read into.
	if (cs->cs_buf == NULL)
	{
		cs->cs_buf = (char *)Alloc(sizeof(char) * LINEBUF_BLOCK_SIZE);
		memset(cs->cs_buf, 0, sizeof(char) * LINEBUF_BLOCK_SIZE);
		cs->cs_buf_point = 0;
		cs->cs_buf_size = LINEBUF_BLOCK_SIZE;
	} else if (int(cs->cs_buf_size)  < cs->cs_buf_point + 1)
	{
		cs->cs_buf_size = cs->cs_buf_size + LINEBUF_BLOCK_SIZE;
		Resize((void *)cs->cs_buf, cs->cs_buf_size);
		memset(&(cs->cs_buf[cs->cs_buf_point]), 0, sizeof(char) * (cs->cs_buf_size - cs->cs_buf_point));
	}
	
	// Recv a single char into the cs_buf
	num = recv(cs->cs_fdin, &(cs->cs_buf[cs->cs_buf_point]), sizeof(char), 0);
	if (num <= 0)
	{
		// socket closed on us, we need to quit
		Client.quit = 1;
		goto cli_socket_handle_input_error;
	}
	
	DPRINT("cli_socket_handle_input: cs_buf is \"%s\"\n", cs->cs_buf);
	
	// Check to see if we just read in a newline character
	// also increment the point
	if (cs->cs_buf[cs->cs_buf_point++] == '\n')
	{
		DPRINT("cli_socket_handle_input: Found newline.\n");
		// increment the completed line
		cs->cs_line++;
		// Finished reading in a whole line, call the handler function
		cmd_buf = strdup(cs->cs_buf);
		// Cleanup the existing buffer, in case this gets interrupted.
		Free(cs->cs_buf);
		cs->cs_buf = NULL;
		DPRINT("cli_socket_handle_input: line is \"%s\"\n", cmd_buf);
		ExecuteCommand(cmd_buf);
		// Free the cmd_buf
		Free(cmd_buf);
		if (cs->cs_nprom > 0)
		{
			PrintOutLine("%s", cli_get_prompt());
			fflush(cs->cs_fpout);
		}
	}

	// Indicate command handling finished, reenable input
	cs->cs_handling = 0;

	// Ignore stream errors for socket - we don't want to handle them in the
	// cli layer
	cs->cs_err = 0;
	
	DPRINT("Exit cli_socket_handle_input\n");
	
	return 0;
	
cli_socket_handle_input_error:
	DPRINT("Error exit cli_socket_handle_input\n");

	// Reenable input
	cs->cs_handling = 0;

	return 1;
}

