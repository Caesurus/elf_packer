    .text
    .align 2
    .globl  z_trampo
    .type   z_trampo,@function
z_trampo:
    mr      %r6, %r3      # save fn ptr
    mr      %r1, %r4      # set new SP
    mr      %r3, %r5      # arg0 = original r2
    mtctr   %r6
    bctr
    .size   z_trampo, .-z_trampo
