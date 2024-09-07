.section .multiboot
/* Declare constants for the multiboot header. */
.set ALIGN,      1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,    1<<1             /* provide memory map */
.set VIDEO_MODE, 1<<2            /* enable video mode */
.set FLAGS,    ALIGN | MEMINFO | VIDEO_MODE  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */

multiboot:
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.long 0,0,0,0,0
.long 0
/* Width, Height, Pixel Depth (32 bit is true color) */
.long 1280, 720, 32

/*
 * Create a 16 Kb stack, will later move %esp to stack top
 */
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .rodata
critical_error_message:
.asciz "Critical Error, DoomOS has halted"

.section .text

.global _start
/*
 * We will start at _start (can be configured by linker)
*/
.extern kernel_main
_start:
	/*
	 * Grub is nice and boots us into 32 bit mode however paging is disabled .
	 */
	movl $stack_top, %esp
	
	push %eax
	push %ebx
	// Jump to C code here
	call kernel_main
	jmp ___kernel__printstr____
	cli
1:	hlt
	jmp 1b

.size _start, . - _start
/*
 * In case the jump to C fails, use this to print 
 */
___kernel__printstr____:
	cli #disable interupts
	movl $'A', %ecx
	movl $critical_error_message, %eax
	movl $0xb8000, %ebx

print_loop:	
	cmpb $0, (%eax)
	je end_loop
	movb (%eax), %cl
	movb %cl, (%ebx)
	addl $2, %ebx
	addl $1, %eax
	jmp print_loop
end_loop:
	hlt
	
/*
Set the size of the _start symbol to the current location '.' minus its start.
This is useful when debugging or when you implement call tracing.
*/

