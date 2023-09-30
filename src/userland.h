#ifndef USERLAND_H
#define USERLAND_H

#include <types.h>
#include "paging.h"

#define USERLAND_BASE_ADDRESS 0x5000000

struct userland_data
{
    struct page_directory_entry page_directories[1024];
    struct page_table_entry page_table[1024];
} __attribute__((packed));


struct userlands_data {
    struct userland_data userland_data[128];
    int curent_process;
    u32 active_process[4];
} __attribute__((packed));

extern struct userlands_data *userland_data;

int create_process(int uid, char *data_start);
int switch_to_ring_3(int uid);
int switch_to_process(int uid);

#endif /*   !USERLAND_H */
