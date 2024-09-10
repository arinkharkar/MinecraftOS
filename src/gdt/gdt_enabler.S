.section .text
.global activate_gdt
.extern pgdt
activate_gdt:
    push %ebp
    mov %esp, %ebp
    cli
    lgdt pgdt
    
    jmp $0x10, $fr
fr:

    mov $0x08, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    mov %ebp, %esp
    pop %ebp
    
    ret