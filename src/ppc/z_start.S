    .text
    .align 2
    .globl z_start
    .type  z_start,@function
z_start:
    mr      %r4, %r3         # save fini → r4
    mr      %r3, %r1         # orig SP → r3
    addi    %r1, %r1, -1024  # allocate new stack frame
    bl      z_entry
1:  b       1b
    nop
    .size z_start, .-z_start
