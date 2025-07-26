    .set    mips32
    .set    noreorder
    .text
    .align  2
    .globl  z_syscall
    .ent    z_syscall
z_syscall:
    move    $v0, $a0            # syscall number
    move    $a0, $a1            # arg1
    move    $a1, $a2            # arg2
    move    $a2, $a3            # arg3
    lw      $a3, 0x10($sp)      # arg4
    lw      $t0, 0x14($sp)      # arg5
    lw      $t1, 0x18($sp)      # arg6
    lw      $t2, 0x1c($sp)      # arg7

    addiu   $sp, $sp, -0x20     # allocate stack space
    sw      $t0, 0x10($sp)      # store arg5
    sw      $t1, 0x14($sp)      # store arg6
    sw      $t2, 0x18($sp)      # store arg7
    sw      $v0, 0x1c($sp)      # save syscall number

    lw      $v0, 0x1c($sp)      # reload syscall number
    syscall                     # invoke syscall

    beqz    $a3, no_error       # check for error
    addiu   $sp, $sp, 0x20      # restore stack (in delay slot)

    negu    $v0, $v0            # return -errno

no_error:
    jr      $ra
    nop
.end    z_syscall

