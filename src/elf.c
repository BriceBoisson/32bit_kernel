#include "elf.h"

#include <types.h>
#include "serial.h"
#include "debug.h"

int load_elf(char *elf_data_start)
{
    DEBUG_INFO("LOAD ELF: ", elf_data_start);

    struct elf_header *elf_header = (struct elf_header *) elf_data_start;

	DEBUG_INFO("ELF ENTRY POINT: %d", elf_header->entry_point);
	DEBUG_INFO("PROGRAM HEADER ENTRY POINT: %d", elf_header->program_header_table_offset);
	DEBUG_INFO("NUMBER PROGRAM HEADER ENTRY: %d", elf_header->number_of_program_header_table_entries);

	for (int i = 0; i < elf_header->number_of_program_header_table_entries; i++)
	{
		struct elf_header_table *elf_header_table = (struct elf_header_table *) (elf_data_start + elf_header->program_header_table_offset + sizeof(struct elf_header_table) * i);
		u32 segment_type = elf_header_table->segment_type;

		if (segment_type == LOAD && elf_header_table->p_vaddr < 0x100000)
		{
			memcpy(elf_header_table->p_vaddr, elf_data_start + elf_header_table->p_offset, elf_header_table->p_filesz);

			if (elf_header_table->p_filesz < elf_header_table->p_memsz)
			{
				for (int i = elf_header_table->p_filesz; i < elf_header_table->p_memsz; i++)
					elf_header_table->p_vaddr[elf_data_start + elf_header_table->p_offset + i] = 0;
			}
		}
	}
}
