#ifndef PAGING_H
#define PAGING_H

#include <types.h>

struct page_directory_entry {
    u8 flags;
    u8 available : 4;
    u32 address : 20;
} __attribute__((packed));

struct page_table_entry {
    u8 flags;
    u8 g : 1;
    u8 available : 3;
    u32 address : 20;
} __attribute__((packed));

int make_page(void);

extern struct page_directory_entry page_dir[1024];
extern struct page_table_entry page_table[1024];

#endif  /*  !PAGING_H */
