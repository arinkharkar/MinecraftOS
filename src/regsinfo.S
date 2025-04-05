.global get_eax
get_eax:
    ret

.global get_ebx
get_ebx:
    mov %ebx, %eax
    ret

.global get_ecx
get_ecx:
    mov %ecx, %eax
    ret

.global get_edx
get_edx:
    mov %edx, %eax
    ret

.global get_esi
get_esi:
    mov %esi, %eax
    ret

.global get_edi
get_edi:
    mov %edi, %eax
    ret

.global get_ebp
get_ebp:
    mov %ebp, %eax
    ret

.global get_esp
get_esp:
    mov %esp, %eax
    ret

.global get_eip
get_eip:
    # You can't directly access EIP.
    # Use `call` and `pop` trick:
    call 1f
1:
    pop %eax
    ret

.global get_eflags
get_eflags:
    pushf
    pop %eax
    ret