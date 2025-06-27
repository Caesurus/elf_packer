    .set    mips32
    .set    noreorder
    .text
    .align  2
    .globl  z_syscall
    .ent    z_syscall
z_syscall:
    move    $v0, $a0        # syscall number
    move    $a0, $a1        # arg1
    move    $a1, $a2        # arg2
    move    $a2, $a3        # arg3
    lw      $a3, 16($sp)    # arg4
    syscall
    jr      $ra
    nop
    .end    z_syscall
