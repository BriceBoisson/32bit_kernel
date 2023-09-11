ENTRY(_start)
OUTPUT_FORMAT("elf32-i386")

SECTIONS
{
	.text : ALIGN(CONSTANT(MAXPAGESIZE)) {
		_TEXT_START_ = .;
			*(.multiboot) *(.text)
		_TEXT_END_ = .;
	}

	.data : ALIGN(CONSTANT(MAXPAGESIZE)) {
		_DATA_START_ = .;
			*(.data)
	}

	.bss : ALIGN(CONSTANT(MAXPAGESIZE)) {
		_BSS_START_ = .;
			*(.bss)
		_BSS_END_ = .;
	}
}
