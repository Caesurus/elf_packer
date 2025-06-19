 .set    mips3
 .set    noreorder
 .set    noat
 .text
 .align  2
 .globl  z_start
 .ent    z_start
 z_start:
    bal     1f
    nop                     # delay slot
1:  .cpload $ra
     move    $t3,  $zero
     move    $ra,   $zero
     move    $a1,   $a0         # fini → a1
     move    $a0,   $sp         # stash original SP
     addiu   $sp,   $sp, -1024  # allocate new stack frame
     la      $t9, z_entry
     jalr    $t9
     nop
     j       .
     nop
     .end    z_start
