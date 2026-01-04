MEMORY {
    DTCMRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
    SRAM1 (rwx) : ORIGIN = 0x30000000, LENGTH = 288K
    SRAM4 (rwx) : ORIGIN = 0x38000000, LENGTH = 64K
    AXISRAM (rwx) : ORIGIN = 0x24000000, LENGTH = 512K
    QUADFLASH (rx) : ORIGIN = 0x90000000, LENGTH = 16M
}

SECTIONS {
    . = 0x90000000;
    .text : {
        KEEP(*(.text.entry))
        *(.text)
        *(.rodata)
    } > QUADFLASH
    .bss (NOLOAD) : {
        . = ORIGIN(SRAM1) + 20K;
        _doom_bss_vma_start = .;
        *(.bss)
        . = ALIGN(4);
        _doom_bss_vma_end = .;
    } > SRAM1
    .data : {
        . = ORIGIN(SRAM4);
        _doom_data_vma_start = .;
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        _doom_data_vma_end = .;
    } > SRAM4 AT > QUADFLASH
    _doom_data_lma = LOADADDR(.data);

}
