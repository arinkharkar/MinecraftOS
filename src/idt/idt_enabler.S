.global idt_enable
.extern pidt

idt_enable:
    lidt pidt
    //sti
    ret