/* UDA server main program.

Copyright 2007 by Intrepid Technology, Inc.
Copyright 2007 by Hewlett-Packard.

Written by; Gary Funck <gary@intrepid.com>
Reviewed by: Brian Wibecan and Tanya Klinchina, HP

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

(1) Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
(2) Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
(3) Neither the name of Intrepid Technology, or Hewlett-Packard
nor the names of its contributors may be used to endorse or promote
products derived from this software without specific prior written
permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  */

#include <ansidecl.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <wait.h>
#include "uda-defs.h"
#include "uda-types.h"
#include "uda-rmt-utils.h"
#include "uda-server.h"

#define UDA_SERVICE "/tmp/uda_service"

/* Create a socket associated with SERVICE_NAME, bind to it
   and return associated file descriptor.  */

static int
udaserver_bind (const char *service_name)
{
  struct sockaddr_un saun;
  int sock_fd;
  socklen_t len;
  /* Create a UNIX domain socket.  */
  if ((sock_fd = socket (AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
      perror ("socket");
      abort ();
    }
  /* Create named socket.  Delete it if it already exists.  */
  unlink (service_name);
  /* Bind to the named socket.  */
  saun.sun_family = AF_UNIX;
  strcpy (saun.sun_path, service_name);
  len = sizeof (saun.sun_family) + strlen (saun.sun_path);
  if (bind (sock_fd, (struct sockaddr *) &saun, len) < 0)
    {
      perror ("bind");
      abort ();
    }
  return sock_fd;
}

/* Listen on SOCK_FD, and return file descriptor,
   associated with the new connection.  */

static int
udaserver_accept (const int sock_fd)
{
  struct sockaddr_un fsaun;
  socklen_t len;
  int connect_fd;
  /* Listen to socket until client connects.  */
  if (listen (sock_fd, 1) < 0)
    {
      perror ("listen");
      abort ();
    }
  /* Accept client connection.  */
  len = sizeof (struct sockaddr_un);
  if ((connect_fd = accept (sock_fd, (struct sockaddr *) &fsaun, &len)) < 0)
    {
      perror ("accept");
      abort ();
    }
  return connect_fd;
}

int
main (int ARG_UNUSED (argc), char ARG_UNUSED (*argv[]))
{
  int sock_fd, connect_fd;
  int child_pid;
  const char *uda_service;
#ifdef DEBUG
  /* Stderr is unbuffered, and stdout is line buffered.  */
  setbuf (stderr, 0);
  setlinebuf (stdout);
  /* Enable malloc() debugging. */
  putenv ((char *) "MALLOC_CHECK_=2");
#endif /* DEBUG */
  /* Bind to the named socket.  */
  uda_service = getenv ("UDA_SERVICE");
  if (!uda_service)
    uda_service = UDA_SERVICE;
  sock_fd = udaserver_bind (uda_service);
  /* Ignore child process termination.   */
  signal (SIGCHLD, SIG_IGN);
  /* Accept client connections, and create a server process
     to handle them.  */
  for (;;)
    {
      connect_fd = udaserver_accept (sock_fd);
      child_pid = fork ();
      if (child_pid < 0)
	{
	  perror ("fork");
	  abort ();
	}
      else if (child_pid == 0)
	{
	  /* server */
	  close (sock_fd);
	  uda_server (connect_fd);
	  /* We shouldn't get here.  */
	  abort ();
	}
      close (connect_fd);
    }
}
