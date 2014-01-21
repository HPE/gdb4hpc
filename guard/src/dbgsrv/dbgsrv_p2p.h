/*********************************************************************************\
 * dbgsrv_p2p.h
 *
 * © 2012-2013 Cray Inc.  All Rights Reserved.
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
 *********************************************************************************/



#ifndef _DBGSRV_P2P_H
#define _DBGSRV_P2P_H

typedef struct
{
	char		*nid;
	unsigned short	port;
} rankInfoEntry_t;

typedef struct
{
	char 		*id;
	int		nrank;
	rankInfoEntry_t	**rank;
} appInfo_t;

struct appList
{
	appInfo_t	app;
	struct appList	*next;
};
typedef struct appList appList_t;

int LoadAppInfo(appInfo_t *);
int UnloadAppInfo(appInfo_t *);

#endif /* _DBGSRV_P2P_H */
