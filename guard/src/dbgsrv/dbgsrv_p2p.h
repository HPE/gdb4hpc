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
