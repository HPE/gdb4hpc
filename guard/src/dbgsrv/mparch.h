/*
** MP Architecture definitions
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _MPARCH_H
#define _MPARCH_H

#define MPARCH_UNKNOWN		0
#define MPARCH_SERIAL		1
#define MPARCH_POE		2
#define MPARCH_MPICH		3
#define MPARCH_MPICH_MPD	4
#define MPARCH_MPICH_GM		5
#define MPARCH_MPICH_P4		6
#define MPARCH_P4MASTER		7
#define MPARCH_P4SLAVE		8
#define MPARCH_OMPI		9
#define MPARCH_ALPS		10
#define MPARCH_MRNET	        11

struct mp_arch
{
	char *	mp_str;
	int	mp_arch;
};

#endif /* !_MPARCH_H */
