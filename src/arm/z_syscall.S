 .text
 .arch armv7-a
 .align 4
 .global z_syscall
 .type   z_syscall, %function
 z_syscall:
     /* args 5–7 are on the stack at [sp], [sp+4], [sp+8] */
     mov     r7, r0        /* syscall number */
     mov     r0, r1        /* arg1 */
     mov     r1, r2        /* arg2 */
     mov     r2, r3        /* arg3 */
     ldr     r3, [sp, #0]  /* arg4 */
     ldr     r4, [sp, #4]  /* arg5 */
     ldr     r5, [sp, #8]  /* arg6 */
     svc     #0
     bx      lr
