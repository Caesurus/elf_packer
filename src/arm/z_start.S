	.text
	.arch armv7-a
	.align	4
	.globl	z_start
	.hidden	z_start
	.type	z_start,%function
z_start:
    mov     r11, #0
    mov     lr,  #0
    mov     r1,  r0
    mov     r0,  sp
    bl      z_entry
	/* Should not reach. */
    wfi

