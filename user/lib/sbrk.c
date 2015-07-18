/*
 * Ensimag - Projet système
 * Copyright (C) 2012 - Damien Dejean <dam.dejean@gmail.com>
 *
 * Change the location of the program break, which defines the end of the
 * process' data segment (ie, the program break is the first location after
 * the end of the uninitialized data segment). Increasing the program break
 * has the effect of allocating memory to the process; decreasing the break
 * frees memory.
 */

/*****************************************************************************
 * NOTE:
 * The sbrk implementation provided below works on a fixed-size heap. This is
 * provided as a stub to allow you to work without sbrk in a first time. But to
 * have a feature complete project, you'll have to implement it as a syscall.
 * See "man sbrk" for more details on specifications.
 *****************************************************************************/

/* Import ptrdiff_t type */
#include "types.h"

extern void *syscall_sbrk(int increment);

/**
 * Increment the program's data space by <increment> bytes. Calling sbrk()
 * with an increment of 0 can be used to find the current location of the
 * program break.
 *
 * @param increment the number of bytes to extend the data space
 * @return the previous program break on success. If the break was increased
 *         then this value is a pointer to the start of the newly allocated
 *         memory.
 *         (void *)-1 on error (ie: no memory available)
 *
 * @note Extract from man sbrk. Implementation example on a fixed size heap
 * into shared/malloc.c.h.
 */
void *sbrk(ptrdiff_t increment)
{
        return syscall_sbrk((int) increment);
}

