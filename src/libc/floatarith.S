.section .text


.global __addsf3

__addsf3:
  #  push %ebp
  #  mov %esp, %ebp
    fld 4(%esp) #loads 1st number and puts it on the fpu stack
    fld 8(%esp) #loads 2nd number and puts it on the fpu stack
    faddp #adds the 2 numbers
    fstp 4(%esp) #stores the result
    mov 4(%esp), %eax #moves the result to the return register
 #   pop %ebp
    ret #returns

.global __mulsf3

__mulsf3:
  #  push %ebp
  #  mov %esp, %ebp
    fld 4(%esp) #loads 1st number and puts it on the fpu stack
    fld 8(%esp) #loads 2nd number and puts it on the fpu stack
    fmulp #multiplies the 2 numbers
    fstp 4(%esp) #stores the result
    mov 4(%esp), %eax #moves the result to the return register
  #  pop %ebp
    ret #returns

.global __divsf3

__divsf3:
   # push %ebp
   # mov %esp, %ebp
    fld 4(%esp) #loads 1st number and puts it on the fpu stack
    fld 8(%esp) #loads 2nd number and puts it on the fpu stack
    fdivp # divides the 2 numbers
    fstp 4(%esp) #stores the result
    mov 4(%esp), %eax #moves the result to the return register
 #   pop %ebp
    ret #returns

.global __floatunsisf

__floatunsisf:
   # push %ebp
   # mov %esp, %ebp
    fild 4(%esp) #loads 1st number, converts it to a float, and puts it on the fpu stack
    mov 4(%esp), %eax #moves the result to the return register
   # pop %ebp
    ret #returns

.global __fixsfsi

__fixsfsi:
  #  push %ebp
  #  mov %esp, %ebp
    fist 4(%esp) #loads 1st number, converts it to a float, and puts it on the fpu stack
    mov 4(%esp), %eax #moves the result to the return register
 #   pop %ebp
    ret #returns

#computes y * log₂(x) 

.global ytimeslogx

ytimeslogx:
  fld 8(%esp)
  fld 4(%esp)
  fyl2x
  movl 8(%esp), %eax
  ret

.global logbase2x
logbase2x:
  fld1
  fld 4(%esp)
  fyl2x
  movl 4(%esp), %eax
  ret

.global twoxminus1

twoxminus1:
  fld 4(%esp)
  f2xm1
  mov 4(%esp), %eax
  ret

.global pow
pow:
  fld 8(%esp)
  fld 4(%esp)
  fyl2x
  fld1
  fld %st(1)
  fprem
  f2xm1
  fadd
  fscale
  fxch %st(1)
  fstp 4(%esp)
  mov 4(%esp), %eax
  ret

.global _internalsin
_internalsin:
  fld 4(%esp)
  fsin
  fstl 4(%esp)
  mov 4(%esp), %eax
  ret