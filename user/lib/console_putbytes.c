/*
 * Ensimag - Projet système
 * Copyright (C) 2012 - Damien Dejean <dam.dejean@gmail.com>
 *
 * Stub for console_putbytes system call.
 */

#include "syscall.h"

extern void printf_user(const char *s, int len);

void console_putbytes(const char *s, int len) {
        (void)s;
        (void)len;
        /** To be implemented */
	if ((unsigned)s>=0x40000000)
		printf_user(s, len);
}

