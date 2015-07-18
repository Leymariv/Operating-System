	.globl chprio //1
chprio:
	movl 4(%esp), %ecx
	movl 8(%esp), %edx
	movl $1, %eax
	int $49
	ret
	
	.globl clock_settings //2
clock_settings:
	movl 4(%esp), %ecx
	movl 8(%esp), %edx
	movl $2, %eax
	int $49
	ret

	.globl cons_echo //3
cons_echo:
	movl 4(%esp), %ecx
	movl $3, %eax
	int $49
	ret

	.globl cons_read //4
cons_read:
	movl 4(%esp), %ecx
	movl 8(%esp), %edx
	movl $4, %eax
	int $49
	ret

	.globl cons_write //5
cons_write:	
	movl 4(%esp), %ecx
	movl 8(%esp), %edx
	movl $5, %eax
	int $49
	ret
	
	.globl current_clock //6
current_clock:
	movl $6, %eax
	int $49
	ret

	.globl exit //7
exit:
	movl 4(%esp), %ecx
	movl $7, %eax
	int $49
	ret

	.globl getpid //8
getpid:
	movl $8, %eax
	int $49
	ret

	.globl getprio //9
getprio:
	movl 4(%esp), %ecx
	movl $9, %eax
	int $49
	ret

	.globl kill //10
kill:
	movl 4(%esp), %ecx
	movl $10, %eax
	int $49
	ret

	.globl pcount //11
pcount:
	movl 4(%esp), %ecx
	movl 8(%esp), %edx
	movl $11, %eax
	int $49
	ret

	.globl pcreate //12
pcreate:
	movl 4(%esp), %ecx
	movl $12, %eax
	int $49
	ret

	.globl pdelete //13
pdelete:
	movl 4(%esp), %ecx
	movl $13, %eax
	int $49
	ret

	.globl preceive //14
preceive:
	movl 4(%esp), %ecx
	movl 8(%esp), %edx
	movl $14, %eax
	int $49
	ret

	.globl preset //15
preset:
	movl 4(%esp), %ecx
	movl $15, %eax
	int $49
	ret

	.globl psend //16
psend:
	movl 4(%esp), %ecx
	movl 8(%esp), %edx
	movl $16, %eax
	int $49
	ret

	.globl scount //17
scount:
	movl 4(%esp), %ecx
	movl $17, %eax
	int $49
	ret

	.globl screate //18
screate:
	movl 4(%esp), %ecx
	movl 8(%esp), %edx
	movl $18, %eax
	int $49
	ret

	.globl sdelete //19
sdelete:
	movl 4(%esp), %ecx
	movl $19, %eax
	int $49
	ret

	.globl sreset //20
sreset:
	movl 4(%esp), %ecx
	movl 8(%esp), %edx
	movl $20, %eax
	int $49
	ret

	.globl signal //21
signal:
	movl 4(%esp), %ecx
	movl $21, %eax
	int $49
	ret

	.globl shm_create //22
shm_create:
	movl 4(%esp), %ecx
	movl $22, %eax
	int $49
	ret

	.globl shm_acquire //23
shm_acquire:
	movl 4(%esp), %ecx
	movl $23, %eax
	int $49
	ret

	.globl shm_release //24
shm_release:
	movl 4(%esp), %ecx
	movl $24, %eax
	int $49
	ret

	.globl start //25
start:
	pushl %esi
	pushl %edi
	movl 12(%esp), %ecx
	movl 16(%esp), %edx
	movl 20(%esp), %esi
	movl 24(%esp), %edi
	movl $25, %eax
	int $49
	popl %edi
	popl %esi
	ret

	.globl try_wait //26
try_wait:
	movl 4(%esp), %ecx
	movl $26, %eax
	int $49
	ret

	.globl wait //27
wait:
	movl 4(%esp), %ecx
	movl $27, %eax
	int $49
	ret

	.globl wait_clock //28
wait_clock:
	movl 4(%esp), %ecx
	movl $28, %eax
	int $49
	ret

	.globl waitpid //29
waitpid:
	movl 4(%esp), %ecx
	movl 8(%esp), %edx
	movl $29, %eax
	int $49
	ret

	.globl printf_user //30
printf_user:
	movl 4(%esp), %ecx
	movl 8(%esp), %edx
	movl $30, %eax
	int $49
	ret

	.globl signaln //31
signaln:
	movl 4(%esp), %ecx
	movl 8(%esp), %edx
	movl $31, %eax
	int $49
	ret

	.globl reboot //32
reboot:
	movl $32, %eax
	int $49
	ret

	.globl ps //33
ps:
	movl $33, %eax
	int $49
	ret

	.globl sinfo //34
sinfo:
	movl $34, %eax
	int $49
	ret

	.globl pinfo //35
pinfo:  
	movl $35, %eax
	int $49
	ret

	.globl cd_gdf //36
cd_gdf:
	movl $36, %eax
	movl 4(%esp), %ecx
	int $49
	ret

	.globl mkdir_gdf //37
mkdir_gdf:
	movl $37, %eax
	movl 4(%esp), %ecx
	int $49
	ret

	.globl rmdir_gdf //38
rmdir_gdf:
	movl $38, %eax
	movl 4(%esp), %ecx
	int $49
	ret	

	.globl get_dcour //39
get_dcour:
	movl $39, %eax
	movl 4(%esp), %ecx
	int $49
	ret

	.globl ls_gdf //40
ls_gdf:
	movl $40, %eax
	movl 4(%esp), %ecx
	int $49
	ret

	.globl touch_gdf //41
touch_gdf:
	movl $41, %eax
	movl 4(%esp), %ecx
	int $49
	ret

	.globl rm_gdf //42
rm_gdf:
	movl $42, %eax
	movl 4(%esp), %ecx
	int $49
	ret
	
	.globl editor_on //43
editor_on:
	movl $43, %eax
	movl 4(%esp), %ecx
	int $49
	ret

	.globl editor_off //44
editor_off:
	movl $44, %eax
	int $49
	ret

	.globl cat_gdf //45
cat_gdf:
	movl $45, %eax
	movl 4(%esp), %ecx
	int $49
	ret

	.globl syscall_sbrk //46
syscall_sbrk:
	movl $46, %eax
	movl 4(%esp), %ecx
	int $49
	ret

	.globl fopen //47
fopen:
	movl $47, %eax
	movl 4(%esp), %ecx
	movl 8(%esp), %edx
	int $49
	ret

	.globl fclose //48
fclose:
	movl $48, %eax
	movl 4(%esp), %ecx
	int $49
	ret

	.globl fread //49
fread:
	movl $49, %eax
	movl 4(%esp), %ecx
	movl 8(%esp), %edx
	movl 12(%esp), %esi
	movl 16(%esp), %edi
	int $49
	ret

	.globl fwrite //50
fwrite:
	movl $50, %eax
	movl 4(%esp), %ecx
	movl 8(%esp), %edx
	movl 12(%esp), %esi
	movl 16(%esp), %edi
	int $49
	ret