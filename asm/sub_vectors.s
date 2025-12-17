    .section .vectors,"ax"
    .globl _start
    .org 0x00000000
_vectors_sub:
    .long 0x06040000
    .long _start
    .rept 6
    .long _start
    .endr

_start:
    lea __bss_start__, %a0
    lea __bss_end__, %a1
    moveq #0, %d0
1:  cmpa.l %a0, %a1
    beq 2f
    move.l %d0, (%a0)+
    bra 1b
2:
    jsr main
halt:
    bra halt

    .extern __bss_start__
    .extern __bss_end__
    .extern main
