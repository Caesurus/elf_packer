    .text
    .align 2
    .globl  z_syscall
    .type   z_syscall,@function
z_syscall:
    mr      %r0, %r3     # syscall number ← first C‐ABI arg (r3)
    mr      %r3, %r4     # arg1 ← second C‐ABI arg (r4)
    mr      %r4, %r5     # arg2 ← third C‐ABI arg (r5)
    mr      %r5, %r6     # arg3 ← fourth C‐ABI arg (r6)
    mr      %r6, %r7     # arg4 ← fifth C‐ABI arg (r7)
    mr      %r7, %r8     # arg5 ← sixth C‐ABI arg (r8)
    mr      %r8, %r9     # arg6 ← seventh C‐ABI arg (r9)
    sc                   # trap to kernel
    blr                  # return; result is in r3 per PPC/Linux ABI
    .size   z_syscall, .-z_syscall
