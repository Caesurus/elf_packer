    .text
    .arch armv7-a
    .align 4
    .global z_trampo
    .type z_trampo, %function
z_trampo:
    mov     r3, r0
    mov     sp,  r1
    mov     r0,  r2
    bx      r3
    /* Should not reach. */
    wfi
