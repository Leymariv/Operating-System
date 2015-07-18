	.globl traitant_horloge
traitant_horloge:
	//sauvegarde des registres scratch
	pushl %eax	 
	pushl %edx 
	pushl %ecx 
	//appel de la fonction C d affichage
	call affiche_temps

	//restauration des registres
	popl %ecx 
	popl %edx 
	popl %eax
	iret

	.globl traitant_keyboard
traitant_keyboard:
	pushl %eax	 
	pushl %edx 
	pushl %ecx 
	call scan_keyboard
	popl %ecx 
	popl %edx 
	popl %eax
	iret
	
	.globl traitant_syscall
traitant_syscall:
	push %eax
	movl $0x18, %eax	
	movl %eax, %ds
	movl %eax, %fs
	movl %eax, %es
	movl %eax, %gs
 	popl %eax	 
	pushl %edx 
	pushl %ecx 
	cmpl $1, %eax
	jne elsif1
	pushl %edx
	pushl %ecx
	call chprio
	popl %ecx
	popl %edx
	jmp fin_syscall
elsif1:
	cmpl $2, %eax
	jne elsif2
	pushl %edx
	pushl %ecx
	call clock_settings
	popl %ecx
	popl %edx
	jmp fin_syscall
elsif2:
	cmpl $3, %eax
	jne elsif3
	pushl %ecx
	call cons_echo
	popl %ecx
	jmp fin_syscall
elsif3:
	cmpl $4, %eax
	jne elsif4
	pushl %edx
	pushl %ecx
	call cons_read
	popl %ecx
	popl %edx
	jmp fin_syscall
elsif4:
	cmpl $5, %eax
	jne elsif5
	pushl %edx
	pushl %ecx
	call cons_write
	popl %ecx
	popl %edx
	jmp fin_syscall
elsif5:	
	cmpl $6, %eax
	jne elsif6
	call current_clock
	jmp fin_syscall
elsif6:
	cmpl $7, %eax
	jne elsif7
	pushl %ecx
	call exit
	popl %ecx
	jmp fin_syscall
elsif7:
	cmpl $8, %eax
	jne elsif8
	call getpid
	jmp fin_syscall
elsif8:
	cmpl $9, %eax
	jne elsif9
	pushl %ecx
	call getprio
	popl %ecx
	jmp fin_syscall
elsif9:
	cmpl $10, %eax
	jne elsif10
	pushl %ecx
	call kill
	popl %ecx
	jmp fin_syscall
elsif10:
	cmpl $11, %eax
	jne elsif11
	pushl %edx
	pushl %ecx
	call pcount
	popl %ecx
	popl %edx
	jmp fin_syscall
elsif11:
	cmpl $12, %eax
	jne elsif12
	pushl %ecx
	call pcreate
	popl %ecx
	jmp fin_syscall
elsif12:
	cmpl $13, %eax
	jne elsif13
	pushl %ecx
	call pdelete
	popl %ecx
	jmp fin_syscall
elsif13:
	cmpl $14, %eax
	jne elsif14
	pushl $0
	pushl %edx
	pushl %ecx
	call preceive
	popl %ecx
	popl %edx
	popl %edx
	jmp fin_syscall
elsif14:
	cmpl $15, %eax
	jne elsif15
	pushl %ecx
	call preset
	popl %ecx
	jmp fin_syscall
elsif15:
	cmpl $16, %eax
	jne elsif16
	pushl %edx
	pushl %ecx
	call psend
	popl %ecx
	popl %edx
	jmp fin_syscall
elsif16:
	cmpl $17, %eax
	jne elsif17
	pushl %ecx
	call scount
	popl %ecx
	jmp fin_syscall
elsif17:
	cmpl $18, %eax
	jne elsif18
	pushl %edx
	pushl %ecx
	call screate
	popl %ecx
	popl %edx
	jmp fin_syscall
elsif18:
	cmpl $19, %eax
	jne elsif19
	pushl %ecx
	call sdelete
	popl %ecx
	jmp fin_syscall
elsif19:
	cmpl $20, %eax
	jne elsif20
	pushl %edx
	pushl %ecx
	call sreset
	popl %ecx
	popl %edx
	jmp fin_syscall
elsif20:
	cmpl $21, %eax
	jne elsif21
	pushl %ecx
	call signal
	popl %ecx
	jmp fin_syscall
elsif21:
	cmpl $22, %eax
	jne elsif22
	pushl %ecx
	call shm_create
	popl %ecx
	jmp fin_syscall
elsif22:
	cmpl $23, %eax
	jne elsif23
	pushl %ecx
	call shm_acquire
	popl %ecx
	jmp fin_syscall
elsif23:
	cmpl $24, %eax
	jne elsif24
	pushl %ecx
	call shm_release
	popl %ecx
	jmp fin_syscall
elsif24:
	cmpl $25, %eax
	jne elsif25
	pushl %edi
	pushl %esi
	pushl %edx
	pushl %ecx
	call start
	popl %ecx
	popl %edx
	popl %esi
	popl %edi
	jmp fin_syscall	
elsif25:
	cmpl $26, %eax
	jne elsif26
	pushl %ecx
	call try_wait
	popl %ecx
	jmp fin_syscall
elsif26:
	cmpl $27, %eax
	jne elsif27
	pushl %ecx
	call wait
	popl %ecx
	jmp fin_syscall
elsif27:
	cmpl $28, %eax
	jne elsif28
	pushl %ecx
	call wait_clock
	popl %ecx
	jmp fin_syscall
elsif28:
	cmpl $29, %eax
	jne elsif29
	pushl %edx
	pushl %ecx
	call waitpid
	popl %ecx
	popl %edx
	jmp fin_syscall
elsif29:
	cmpl $30, %eax
	jne elsif30
	pushl %edx
	pushl %ecx
	call console_putbytes
	popl %ecx
	popl %edx
	jmp fin_syscall
elsif30:
	cmpl $31, %eax
	jne elsif31
	pushl %edx
	pushl %ecx
	call signaln
	popl %ecx
	popl %edx
	jmp fin_syscall
elsif31:
	cmpl $32, %eax
	jne elsif32
	call reboot
	jmp fin_syscall
elsif32:
	cmpl $33, %eax
	jne elsif33
	call ps
	jmp fin_syscall
elsif33:
	cmpl $34, %eax
	jne elsif34
	call sinfo
	jmp fin_syscall
elsif34:
	cmpl $35, %eax
	jne elsif35
	call pinfo
	jmp fin_syscall
elsif35:
	cmpl $36, %eax
	jne elsif36
	pushl %ecx
	call cd_gdf
	popl %ecx
	jmp fin_syscall
elsif36:
	cmpl $37, %eax
	jne elsif37
	pushl $100
	pushl %ecx
	call mkdir_gdf
	popl %ecx
	popl %ecx
	jmp fin_syscall
elsif37:
	cmpl $38, %eax
	jne elsif38
	pushl $100
	pushl %ecx
	call rmdir_gdf
	popl %ecx
	popl %ecx
	jmp fin_syscall
elsif38:
	cmpl $39, %eax
	jne elsif39
	pushl %ecx
	call get_dcour
	popl %ecx
	jmp fin_syscall
elsif39:
	cmpl $40, %eax
	jne elsif40
	pushl %ecx
	call ls_gdf
	popl %ecx
	jmp fin_syscall
elsif40:
	cmpl $41, %eax
	jne elsif41
	pushl $102
	pushl %ecx
	call mkdir_gdf
	popl %ecx
	popl %ecx
	jmp fin_syscall
elsif41:
	cmpl $42, %eax
	jne elsif42
	pushl $102
	pushl %ecx
	call rmdir_gdf
	popl %ecx
	popl %ecx
	jmp fin_syscall
elsif42:
	cmpl $43, %eax
	jne elsif43
	pushl %ecx
	call editor_on
	popl %ecx
	jmp fin_syscall
elsif43:
	cmpl $44, %eax
	jne elsif44
	call editor_off
	jmp fin_syscall
elsif44:
	cmpl $45, %eax
	jne elsif45
	pushl %ecx
	call cat_gdf
	popl %ecx
	jmp fin_syscall
elsif45:
	cmpl $46, %eax
	jne elsif46
	pushl %ecx
	call syscall_sbrk
	popl %ecx
	jmp fin_syscall
elsif46:
	cmpl $47, %eax
	jne elsif47
	pushl %edx
	pushl %ecx
	call fopen_gdf
	popl %ecx
	popl %edx
	jmp fin_syscall
elsif47:
	cmpl $48, %eax
	jne elsif48
	pushl %ecx
	call fclose_gdf
	popl %ecx
	jmp fin_syscall
elsif48:
	cmpl $49, %eax
	jne elsif49
	pushl %edi
	pushl %esi
	pushl %edx
	pushl %ecx
	call fread_gdf
	popl %ecx
	popl %edx
	popl %esi
	popl %edi
	jmp fin_syscall
elsif49:
	cmpl $50, %eax
	jne elsif50
	pushl %edi
	pushl %esi
	pushl %edx
	pushl %ecx
	call fwrite_gdf
	popl %ecx
	popl %edx
	popl %esi
	popl %edi
	jmp fin_syscall
elsif50:
	jmp fin_syscall
fin_syscall:
	push %eax
	movl $0x4b, %eax	
	movl %eax, %ds
	movl %eax, %fs
	movl %eax, %es
	movl %eax, %gs
 	popl %eax
	popl %ecx 
	popl %edx 
	iret
