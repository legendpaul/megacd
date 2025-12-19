ENTRY(_start)
MEMORY {
  ip : ORIGIN = 0x00000000, LENGTH = 0x00000800
}
SECTIONS {
  .text : { *(.text*) *(.rodata*) } > ip
  .data : { *(.data*) } > ip
  .bss (NOLOAD) : { *(.bss*) *(COMMON) } > ip
}
