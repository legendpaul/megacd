ENTRY(_start)
MEMORY {
  ip : ORIGIN = 0x00000000, LENGTH = 0x00000800
}
SECTIONS {
  /*
   * The IP must fit entirely within a 2 KB sector. Strip out any debug or
   * metadata sections the assembler might emit so only the intended loader
   * bytes are placed in the limited region.
   */
  /DISCARD/ : { *(.comment*) *(.note*) *(.debug*) *(.gnu*) *(.eh_frame*) }

  .text : { *(.text*) *(.rodata*) } > ip
  .data : { *(.data*) } > ip
  .bss (NOLOAD) : { *(.bss*) *(COMMON) } > ip
}
