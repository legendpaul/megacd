ENTRY(_start)
MEMORY {
  ip : ORIGIN = 0x00000000, LENGTH = 0x00000800
}
SECTIONS {
  /*
   * The IP must fit entirely within a 2 KB sector. Strip out any debug or
   * metadata sections the assembler might emit so only the intended loader
   * bytes are placed in the limited region. Some toolchains also emit stab
   * or relocation sections even for raw assembly, so discard those too to
   * keep the output strictly to the loader bytes.
   */
  /DISCARD/ : {
    *(.comment*) *(.note*) *(.debug*) *(.gnu*) *(.eh_frame*)
    *(.stab*) *(.rel*) *(.rela*) *(.note.GNU-stack)
  }

  .text : { *(.text*) *(.rodata*) } > ip
  .data : { *(.data*) } > ip
  .bss (NOLOAD) : { *(.bss*) *(COMMON) } > ip

  /* Fail the link if anything tries to push us past the single-sector cap. */
  _ip_size = SIZEOF(.text) + SIZEOF(.data) + SIZEOF(.bss);
  ASSERT(_ip_size <= LENGTH(ip), "IP image exceeds 2 KB sector budget")
}
