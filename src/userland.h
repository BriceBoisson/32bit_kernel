#ifndef USERLAND_H
#define USERLAND_H

#include <types.h>
#include "paging.h"

#define USERLAND_BASE_ADDRESS 0x5000000

struct userland_data
{
    struct page_directory_entry page_directories[1024];
    struct page_table_entry page_table[1024];
};


struct userlands_data {
    struct userland_data userland_data[128];
    u32 page_dir[4];
};

extern struct userlands_data *userland_data;

void userland(void);

#endif /*   !USERLAND_H */
