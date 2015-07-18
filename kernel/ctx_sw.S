
	.globl ctx_sw
	// Structure de la pile en entree :
	//   %esp + 4 : adresse de l'ancien contexte
	//   %esp + 8 : adresse du nouveau contexte
	
ctx_sw:
	// Sauvegarde de l'ancien contexte	
	movl 4(%esp), %eax
	movl %ebx, 0(%eax)
	movl %esi, 4(%eax)
	movl %edi, 8(%eax)
	movl %esp, 12(%eax)
	movl %ebp, 16(%eax)
    
	// Restauration du nouveau contexte
	//restauration des registres non scratch : %ebx, %esi, %edi, %esp et %ebp
	movl 8(%esp), %eax
	movl 0(%eax), %ebx
	movl 4(%eax), %esi
	movl 8(%eax), %edi
	movl 12(%eax), %esp
	movl 16(%eax), %ebp
	
	// On met à jour le registre ESP0
	movl $0x20004, %ecx
	movl 24(%eax), %edx
	movl %edx, (%ecx)

	// On met le nouveau CR3 dans tss+28 (0x2001C)
	movl $0x2001C, %ecx
	movl 20(%eax), %edx
	movl %edx, (%ecx)

	// On met le nouvea CR3 dans le registre CR3 du processeur
	movl %edx, %cr3

	ret