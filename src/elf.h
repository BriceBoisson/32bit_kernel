#ifndef ELF_H
#define ELF_H

#include <types.h>

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

enum elf_segment_type {
    NULL = 0,
    LOAD = 1,
    DYNAMIC = 2,
    INTERP = 3,
    NOTE = 4
};

int load_elf(char *elf_data_start, int uid);

#endif /*   !ELF_H */
