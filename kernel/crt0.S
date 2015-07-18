/*
 * crt0.S
 *
 * Copyright (C) 2003-2007 by Simon Nieuviarts
 *
 * Kernel entry point.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "start.h"

	.text
	.comm   first_stack,FIRST_STACK_SIZE,32
	.comm   multiboot_magic,4,4
	.comm   multiboot_info,4,4

	/* The entry point must be before the multiboot header. */
.global entry
entry:
	/* Setup the code segment. */
	jmp	entry2

	/* The multiboot header. */
	.p2align 2
0:	.long	0x1BADB002		/* magic number. */
	.long	0x00010002		/* flags. */
	.long	-0x1BADB002-0x00010002	/* checksum. */
	.long	0b
	.long	_start
	.long	_edata
	.long	__bss_end
	.long	entry

	/*
	 * This global label is here to realign the debugger on the beginning
	 * of the code
	 */
entry2:
	movl	%eax,multiboot_magic	/* It can be verified later. */
	movl	%ebx,multiboot_info	/* It can be used later. */

	/* We have to set up a stack. */
	leal	first_stack,%esp
	addl	$FIRST_STACK_SIZE,%esp
	xorl	%ebp,%ebp

	/* Clear the flags. */
	pushl	$0
	popfl
	/* Note that the Interrupt Flag is cleared : interrupts are disabled. */

	/* Blank all uninitialized memory */
	movl	$_edata,%edi
	xorl	%eax,%eax
0:	movl	%eax,(%edi)
	addl	$4,%edi
	cmpl	$mem_heap_end,%edi
	jb	0b

	/* Setup the processor tables. */
	call	cpu_init

	/* To support C++, should run __CTOR_LIST__ just now */

	/* Jumps into the kernel starting code. */
	call	kernel_start

	/* This function should never exit. If it happens, reboot. */
	call	reboot

.global setup_pgtab
setup_pgtab:
/*
 * Setup page tables. The first 256MB are mapped for kernel only.
 * Note that the first page is protected, to catch null pointers.
 */
    leal    pgtab,      %eax
    xorl    %edi,       %edi
    movl    %edi,       (%eax)
    addl    $4,         %eax
    movl    $64*1024-1, %edx
    movl    $0x1007,    %edi
0:  movl    %edi,       (%eax)
    addl    $4,         %eax
    addl    $0x1000,    %edi
    decl    %edx
    jnz     0b

    call setup_rodata

	/* Enable paging */
	leal	pgdir,%eax
	/* CR3 = Base Address of the Page Directory */
	/* Cf. 3.6.3 Intel Architecture Software Developer's Manual Volume 3 */
	movl	%eax,%cr3
	/* Task State Segment, CR3 field must be initialized to Base Address of the Page Directory */
	/* Cf. 3.6.3 Intel Architecture Software Developer's Manual Volume 3 */
	movl	%eax,tss+28
	/* PG (paging) flag, bit 31 of CR0 : must to be set to active paging */
	/* Cf. 6.2.1 Intel Architecture Software Developer's Manual Volume 3 */
	movl	%cr0,%eax  
	orl	$0x80010000,%eax
	movl	%eax,%cr0

	jmp	0f
0:	movl	$0f,%eax
	jmp	*%eax
0:	ret

/* Unimplemented functions.
You have to implement them it in ".c" files. As these symbols are marked as
"weak", they will no more be used as soon as you write your implementations.
*/
	.weak console_putbytes
console_putbytes:
	ret

	.weak keyboard_data
keyboard_data:
	ret

/* Implement this one only if you want to light LEDs in sync with keyboard's
state. */
	.weak kbd_leds
kbd_leds:
	ret

    .org    0x1000
    /* This address will be mapped at kernel_base(0x10000) + 0x1000=0x101000 */
    /* page directory */
    /* Cf. 3.6.2 Intel Architecture Software Developer's Manual Volume 3 */
    .global pgdir
pgdir:
    /* Directory 0 to 64 : All the memory space is mapped for the kernel */
    /* Cf. 3.6.4 Intel Architecture Software Developer's Manual Volume 3 */
    .long   0x102003
    .long   0x103003
    .long   0x104003
    .long   0x105003
    .long   0x106003
    .long   0x107003
    .long   0x108003
    .long   0x109003
    .long   0x10a003
    .long   0x10b003
    .long   0x10c003
    .long   0x10d003
    .long   0x10e003
    .long   0x10f003
    .long   0x110003
    .long   0x111003
    .long   0x112003
    .long   0x113003
    .long   0x114003
    .long   0x115003
    .long   0x116003
    .long   0x117003
    .long   0x118003
    .long   0x119003
    .long   0x11a003
    .long   0x11b003
    .long   0x11c003
    .long   0x11d003
    .long   0x11e003
    .long   0x11f003
    .long   0x120003
    .long   0x121003
    .long   0x122003
    .long   0x123003
    .long   0x124003
    .long   0x125003
    .long   0x126003
    .long   0x127003
    .long   0x128003
    .long   0x129003
    .long   0x12a003
    .long   0x12b003
    .long   0x12c003
    .long   0x12d003
    .long   0x12e003
    .long   0x12f003
    .long   0x130003
    .long   0x131003
    .long   0x132003
    .long   0x133003
    .long   0x134003
    .long   0x135003
    .long   0x136003
    .long   0x137003
    .long   0x138003
    .long   0x139003
    .long   0x13a003
    .long   0x13b003
    .long   0x13c003
    .long   0x13d003
    .long   0x13e003
    .long   0x13f003
    .long   0x140003
    .long   0x141003
    /* Fill with zeros the end of the table */
    .fill   1024-64,4,0

    /* Page table: will contain all mappings for the kernel space */
    .org    0x2000
    .global pgtab
pgtab:
    .org    0x42000
