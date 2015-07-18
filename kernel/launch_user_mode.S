	.global launch_user_mode
launch_user_mode:
	push %eax
	movl $0x4b, %eax	
	movl %eax, %ds
	movl %eax, %fs
	movl %eax, %es
	movl %eax, %gs
 	popl %eax
	pushl $0x4b
	pushl $0x80000ff8
	pushl $0x202
	pushl $0x43
	pushl $0x40000000
	iret
