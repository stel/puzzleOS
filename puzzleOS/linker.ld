ENTRY (multiboot_entry_point)

SECTIONS{
    . = 0x00100000;

    .text :{
		code = .; _code = .; __code = .;
        *(.text)
    }

    .data ALIGN (0x1000) : {
		data = .; _data = .; __data = .;
        *(.data)
		*(.rodata)
    }

    .bss : {
		bss = .; _bss = .; __bss = .;
		*(.bss)
    }
	
	kernel_end = .; _kernel_end = .;
}