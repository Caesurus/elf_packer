    .set    mips32
    .set    noreorder
    .text
    .align  2
    .globl  z_trampo
    .ent    z_trampo
z_trampo:
    move    $t3, $a0       # save fn ptr
    move    $sp, $a1       # set new SP
    move    $a0, $a2       # arg0 = original r2
    jr      $t3
    nop
    .end    z_trampo
