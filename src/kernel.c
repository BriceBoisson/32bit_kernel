#include <types.h>

#include "gdt.h"
#include "idt.h"
#include "pic_controler.h"
#include "serial.h"
#include "debug.h"
#include "tss.h"
#include "launch_process.h"

struct elf_header
{
    u32 magic_number;
    u8 bus_size;
    u8 endianess;
    u8 elf_header_version;
    u8 os_abi;
    u32 unused[2];
    u16 type;
    u16 isa;
    u32 elf_version;
    u32 entry_point;
    u32 program_header_table_offset;
    u32 section_header_table_offset;
    u32 flags;
    u16 header_size;
    u16 program_header_table_entry_size;
    u16 number_of_program_header_table_entries;
    u16 size_of_program_header_table_entries;
    u16 number_of_section_header_table_entries;
    u16 index_section_header_table_with_section_names;
} __attribute__((packed));

struct elf_header_table
{
    u32 segment_type;
    u32 p_offset;
    u32 p_vaddr;
    u32 undefined;
    u32 p_filesz;
    u32 p_memsz;
    u32 flags;
    u32 alignment;
} __attribute__((packed));

int read_cdrom(u16 port, bool slave, u32 lba, u32 sectors, u16 *buffer);

extern char _binary_a_out_start[];
extern char _binary_a_out_end[];
extern char _binary_a_out_size[];

void main(void)
{
	char *data_start     = &_binary_a_out_start;
    char *data_end       = _binary_a_out_end;
    size_t data_size  = (size_t)_binary_a_out_size;

	DEBUG_INFO("data_start: %d", _binary_a_out_start);
	DEBUG_INFO("data_start: %d", _binary_a_out_end);
	DEBUG_INFO("data_start: %d", _binary_a_out_size);

	// write_serial("\n");
	// write_serial("mem ");
	// write_serial_nb(data_start, false);
	// write_serial(": ");
	// for (char * j = data_start; j < data_start + 0x1005; j++)
	// {
	// 	if ((u32) j % 25 == 0)
	// 	{
	// 		write_serial("\n");
	// 		write_serial("mem ");
	// 		write_serial_nb(j, false);
	// 		write_serial(": ");
	// 		write_serial_nb((u8) *j, false);
	// 	}
	// 	else
	// 	{
	// 		write_serial(": ");
	// 		write_serial_nb((u8) *j, false);
	// 	}
	// }

	struct elf_header *elf_header = (struct elf_header *) data_start;

	write_serial("ELF: ");
	char *magic_number = (char *) &elf_header->magic_number;
	DEBUG_INFO("ELF: %c%c%c%c", magic_number[0], magic_number[1], magic_number[2], magic_number[3]);
	DEBUG_INFO("ELF ENTRY POINT: %d", elf_header->entry_point);
	DEBUG_INFO("PROGRAM HEADER ENTRY POINT: %d", elf_header->program_header_table_offset);
	DEBUG_INFO("NUMBER PROGRAM HEADER ENTRY: %d", elf_header->number_of_program_header_table_entries);

	for (int i = 0; i < elf_header->number_of_program_header_table_entries; i++)
	{
		struct elf_header_table *elf_header_table = (struct elf_header_table *) (data_start + elf_header->program_header_table_offset + sizeof(struct elf_header_table) * i);
		DEBUG_INFO("ELF HEADER TABLE OFFSET: %d, %d", elf_header_table, (u32) elf_header_table - (u32) data_start);
		char *segment_type = (char *) &elf_header_table->segment_type;
		DEBUG_INFO("ELF HEADER TYPE: %d%d%d%d", segment_type[0], segment_type[1], segment_type[2], segment_type[3]);
		if (segment_type[0] == 1 && elf_header_table->p_vaddr < 0x100000)
		{
			DEBUG_INFO("-------------------");
			DEBUG_INFO("CODE DATA: %d", elf_header_table->p_offset);
			DEBUG_INFO("CODE START: %d", elf_header_table->p_vaddr);
			DEBUG_INFO("CODE SIZE: %d", elf_header_table->p_filesz);
			DEBUG_INFO("ALLOCATION SIZE: %d", elf_header_table->p_memsz);
			DEBUG_INFO("CODE START SECTOR: %d", data_start + elf_header_table->p_offset);
			memcpy(elf_header_table->p_vaddr, data_start + elf_header_table->p_offset, elf_header_table->p_filesz);
			// for (int j = 0; j < 0x22; j++)
			// {
			// 	write_serial_nb(*(data_start + 0x1000 + j), false);
			// 	write_serial(" ");
			// }
			DEBUG_INFO("CODE: %d", *(data_start + elf_header_table->p_offset));
			if (elf_header_table->p_filesz < elf_header_table->p_memsz)
			{
				for (int i = elf_header_table->p_filesz; i < elf_header_table->p_memsz; i++)
				{
					elf_header_table->p_vaddr[data_start + elf_header_table->p_offset + i] = 0;
				}
			}
			DEBUG_INFO("-------------------");
		}
	}

	u32 *add = 0x30000;
	DEBUG_INFO("data_start: %d", *add);


	// for (int i = 0; i < 100; i++)
	// {
	// 	DEBUG_INFO("bin(%d) %d", i, data_start[i]);
	// }

	DEBUG_INFO("Entering Main Function");

	// for (int i = 0; i < 100000; i++)
	// {
	// 	u16 bufffer[2048];
	// 	int read_cdrom_ret = read_cdrom(0x0172, 0, i, 1, bufffer);

	// 	// DEBUG_INFO("read_cdrom returned %d", read_cdrom_ret);
	// 	DEBUG_INFO("read_cdrom returned %s", bufffer);
	// }

	launch_process(0x28, 0x30000, 0x20, 0x18, 0x20);

	for (;;)
	{
		DEBUG_INFO("Waiting for an interrupt");
		asm volatile ("hlt");
	}
}

void kernel_main(void)
{
	init_serial();

	DEBUG_INFO("Starting kernel");

	init_gdt();
	init_idt();
	pic_init();
	make_page();
	asm volatile ("sti");

	main();
}
