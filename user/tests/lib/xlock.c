/**
 * Wrapper sur les structures de verrouillage disponibles dans le kernel:
 * les files de messages, ou les sémaphores.
 */


#include "sysapi.h"

#if defined WITH_SEM
/** Implémentation de verrous en se basant sur les sémaphores. */
void xwait(struct sem *s)
{
        assert(wait(s->sem) == 0);
}

void xsignal(struct sem *s)
{
        assert(signal(s->sem) == 0);
}

void xscreate(struct sem *s)
{
        assert((s->sem = screate(0)) >= 0);
}

void xsdelete(struct sem *s)
{
        assert(sdelete(s->sem) == 0);
}


#elif defined WITH_MSG
/** Implémentation de verrous en se basant sur les files de message. */
void xwait(struct sem *s)
{
        assert(preceive(s->fid, 0) == 0);
}

void xsignal(struct sem *s)
{
        int count;
        assert(psend(s->fid, 1) == 0);
        assert(pcount(s->fid, &count) == 0);
        //assert(count == 1); XXX
        assert(count < 2);
}

void xscreate(struct sem *s)
{
        assert((s->fid = pcreate(2)) >= 0);
}

void xsdelete(struct sem *s)
{
        assert(pdelete(s->fid) == 0);
}


#else
# error "WITH_SEM" ou "WITH_MSG" doit être définie.
#endif
