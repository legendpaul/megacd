ENTRY(_start)
MEMORY {
  rom : ORIGIN = 0x000000, LENGTH = 0x400000
}
SECTIONS {
  .text : {
    KEEP(*(.vectors))
    *(.text*)
  } > rom
  .rodata : { *(.rodata*) } > rom
  .data : { *(.data*) } > rom
  .bss (NOLOAD) : {
    __bss_start__ = .;
    *(.bss*) *(COMMON)
    __bss_end__ = .;
  } > rom
}
