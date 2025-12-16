ENTRY(_start)
MEMORY {
  ram : ORIGIN = 0x06000000, LENGTH = 0x00080000
}
SECTIONS {
  .text : {
    KEEP(*(.vectors))
    *(.text*)
  } > ram
  .rodata : { *(.rodata*) } > ram
  .data : { *(.data*) } > ram
  .bss (NOLOAD) : {
    __bss_start__ = .;
    *(.bss*) *(COMMON)
    __bss_end__ = .;
  } > ram
}
