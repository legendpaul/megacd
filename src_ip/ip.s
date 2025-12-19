    .section .text
    .globl _start

/*
 * Mega-CD IP sector (2048 bytes)
 * Keep this loader tiny so it fits in the boot sector. It sets the system
 * to supervisor mode and jumps into the main 68000 program placed at the
 * start of the disc image (MAIN.BIN). The BIOS copies IP.BIN into word RAM
 * and branches to _start after validating the header.
 */

/* Sega CD requires the identification string at the top of the sector. */
header:
    .ascii "SEGADISCSYSTEM  "   /* 0x00: console signature */
    .ascii "SEGA ENTERPRISES"    /* 0x10: developer */
    .ascii "MEGA-CD  MINNKA"     /* 0x20: product name (padded) */
    .ascii "JUE"                 /* 0x30: region flags */

/* Pad out to 0x50 where the BIOS expects the program type descriptor. */
    .org 0x50
    .ascii "BOOT"                /* Program type: bootable IP */
    .ascii "MAIN"                /* Module name */

/* Jump code lives a little further into the sector. */
    .org 0x100
_start:
    move    #0x2700, %sr         /* Supervisor mode, disable ints */
    lea     0x00020000, %a0      /* Word RAM where MAIN.BIN is loaded */
    jmp     (%a0)                /* Hand off to the main program */

/* Fill the remainder of the sector so objcopy emits exactly 2048 bytes. */
    .org 0x800
