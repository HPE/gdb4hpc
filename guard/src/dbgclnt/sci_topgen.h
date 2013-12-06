/* interface of MRNet for Guard */
#ifndef _GUARD_SCI_TOPGEN_H
#define _GUARD_SCI_TOPGEN_H

typedef struct
{
        char *  hostname;
        int     numProcs;
} hostPlacement_t;

typedef struct
{
        int                 numHosts;
        hostPlacement_t *   hostPlacement;
} jobHostPlacement_t;

#endif /* _GUARD_SCI_TOPGEN_H */
