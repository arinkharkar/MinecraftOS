.align   4
.extern interrupt_handler

.macro ISR_HANDLER isr_num
    cli                       # Disable interrupts
    push $0                    # Push error code
    push $\isr_num             # Push interrupt number (this will be the macro argument)
    pusha                      # Push all general-purpose registers
    call interrupt_handler     # Call the common handler (or change as per your needs)
    popa
    add $8, %esp
    sti
    iret                       # Return from interrupt
.endm


.global isr_error_handler_0
isr_error_handler_0:
    ISR_HANDLER 0

.global isr_error_handler_1
isr_error_handler_1:
    ISR_HANDLER 1

.global isr_error_handler_2
isr_error_handler_2:
    ISR_HANDLER 2

.global isr_error_handler_3
isr_error_handler_3:
    ISR_HANDLER 3

.global isr_error_handler_4
isr_error_handler_4:
    ISR_HANDLER 4

.global isr_error_handler_5
isr_error_handler_5:
    ISR_HANDLER 5

.global isr_error_handler_6
isr_error_handler_6:
    ISR_HANDLER 6

.global isr_error_handler_7
isr_error_handler_7:
    ISR_HANDLER 7

.global isr_error_handler_8
isr_error_handler_8:
    ISR_HANDLER 8

.global isr_error_handler_9
isr_error_handler_9:
    ISR_HANDLER 9

.global isr_error_handler_10
isr_error_handler_10:
    ISR_HANDLER 10

.global isr_error_handler_11
isr_error_handler_11:
    ISR_HANDLER 11

.global isr_error_handler_12
isr_error_handler_12:
    ISR_HANDLER 12

.global isr_error_handler_13
isr_error_handler_13:
    ISR_HANDLER 13

.global isr_error_handler_14
isr_error_handler_14:
    ISR_HANDLER 14

.global isr_error_handler_15
isr_error_handler_15:
    ISR_HANDLER 15

.global isr_error_handler_16
isr_error_handler_16:
    ISR_HANDLER 16

.global isr_error_handler_17
isr_error_handler_17:
    ISR_HANDLER 17

.global isr_error_handler_18
isr_error_handler_18:
    ISR_HANDLER 18

.global isr_error_handler_19
isr_error_handler_19:
    ISR_HANDLER 19

.global isr_error_handler_20
isr_error_handler_20:
    ISR_HANDLER 20

.global isr_error_handler_21
isr_error_handler_21:
    ISR_HANDLER 21

.global isr_error_handler_22
isr_error_handler_22:
    ISR_HANDLER 22

.global isr_error_handler_23
isr_error_handler_23:
    ISR_HANDLER 23

.global isr_error_handler_24
isr_error_handler_24:
    ISR_HANDLER 24

.global isr_error_handler_25
isr_error_handler_25:
    ISR_HANDLER 25

.global isr_error_handler_26
isr_error_handler_26:
    ISR_HANDLER 26

.global isr_error_handler_27
isr_error_handler_27:
    ISR_HANDLER 27

.global isr_error_handler_28
isr_error_handler_28:
    ISR_HANDLER 28

.global isr_error_handler_29
isr_error_handler_29:
    ISR_HANDLER 29

.global isr_error_handler_30
isr_error_handler_30:
    ISR_HANDLER 30

.global isr_error_handler_31
isr_error_handler_31:
    ISR_HANDLER 31

.global isr_error_handler_32
isr_error_handler_32:
    ISR_HANDLER 32
.global isr_error_handler_33
isr_error_handler_33:
    ISR_HANDLER 33

.global isr_error_handler_34
isr_error_handler_34:
    ISR_HANDLER 34

.global isr_error_handler_35
isr_error_handler_35:
    ISR_HANDLER 35

.global isr_error_handler_36
isr_error_handler_36:
    ISR_HANDLER 36

.global isr_error_handler_37
isr_error_handler_37:
    ISR_HANDLER 37

.global isr_error_handler_38
isr_error_handler_38:
    ISR_HANDLER 38

.global isr_error_handler_39
isr_error_handler_39:
    ISR_HANDLER 39

.global isr_error_handler_40
isr_error_handler_40:
    ISR_HANDLER 40

.global isr_error_handler_41
isr_error_handler_41:
    ISR_HANDLER 41

.global isr_error_handler_42
isr_error_handler_42:
    ISR_HANDLER 42

.global isr_error_handler_43
isr_error_handler_43:
    ISR_HANDLER 43

.global isr_error_handler_44
isr_error_handler_44:
    ISR_HANDLER 44

.global isr_error_handler_45
isr_error_handler_45:
    ISR_HANDLER 45

.global isr_error_handler_46
isr_error_handler_46:
    ISR_HANDLER 46

.global isr_error_handler_47
isr_error_handler_47:
    ISR_HANDLER 47

.global isr_error_handler_48
isr_error_handler_48:
    ISR_HANDLER 48

.section .data
.global isr_table
isr_table:
    .long isr_error_handler_0
    .long isr_error_handler_1
    .long isr_error_handler_2
    .long isr_error_handler_3
    .long isr_error_handler_4
    .long isr_error_handler_5
    .long isr_error_handler_6
    .long isr_error_handler_7
    .long isr_error_handler_8
    .long isr_error_handler_9
    .long isr_error_handler_10
    .long isr_error_handler_11
    .long isr_error_handler_12
    .long isr_error_handler_13
    .long isr_error_handler_14
    .long isr_error_handler_15
    .long isr_error_handler_16
    .long isr_error_handler_17
    .long isr_error_handler_18
    .long isr_error_handler_19
    .long isr_error_handler_20
    .long isr_error_handler_21
    .long isr_error_handler_22
    .long isr_error_handler_23
    .long isr_error_handler_24
    .long isr_error_handler_25
    .long isr_error_handler_26
    .long isr_error_handler_27
    .long isr_error_handler_28
    .long isr_error_handler_29
    .long isr_error_handler_30
    .long isr_error_handler_31
    .long isr_error_handler_32
    .long isr_error_handler_33
    .long isr_error_handler_34
    .long isr_error_handler_35
    .long isr_error_handler_36
    .long isr_error_handler_37
    .long isr_error_handler_38
    .long isr_error_handler_39
    .long isr_error_handler_40
    .long isr_error_handler_41
    .long isr_error_handler_42
    .long isr_error_handler_43
    .long isr_error_handler_44
    .long isr_error_handler_45
    .long isr_error_handler_46
    .long isr_error_handler_47
    .long isr_error_handler_48