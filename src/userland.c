#include "userland.h"

#include "debug.h"
#include "paging.h"
#include "launch_process.h"

struct userlands_data *userland_data = 0x3020000;

int create_process(int uid, char *data_start)
{
    create_new_userland_page(uid);

    DEBUG_INFO("CREATE PROCESS");
    void *process_page_dir_adress = userland_data->userland_data[uid].page_directories;

    // load cr3
    asm volatile ("                 \
        mov %0, %%eax     \n \
        mov %%eax, %%cr3" : "+r" (process_page_dir_adress));

    // Allocate stack
    allocate_new_page(uid, 0x7FFFF000);
    allocate_new_page(uid, 0x7FFFE000);
    allocate_new_page(uid, 0x7FFFD000);

    // TODO : create data seg by process
    // load elf
    load_elf(data_start, uid);

    return 0;
}

int switch_to_process(int uid)
{
    DEBUG_INFO("SWITCHING TO PROCESS");
    void *process_page_dir_adress = userland_data->userland_data[uid].page_directories;

    // load cr3
    asm volatile ("                 \
        mov %0, %%eax     \n \
        mov %%eax, %%cr3" : "+r" (process_page_dir_adress));

    // TODO : once data by process has been implemented, load the right one
    launch_process(0x28, 0x6000000, 0x20, 0x18, 0x20);
}
