#include "paging.h"

#include "debug.h"
#include "serial.h"
#include "userland.h"

struct page_directory_entry *page_dir; // = 0x3000000; //__attribute__((aligned(4096)));
struct page_table_entry *page_table; // = 0x3010000; //__attribute__((aligned(4096)));
u8 page_avl[131072] = {0};
int page_avl_index = (USERLAND_BASE_ADDRESS >> 12) / 8;
int userland_page_dir_index = 0;

#define KERNEL_PAGE_DIR 0x3000000
#define KERNEL_PAGE_TABLE 0x3010000
#define NB_KERNEL_PAGE_DIR 20

int find_page_avl(void)
{
    for (int i = page_avl_index; i < 131071; i++) {
        for (int j = 0; j < 8; j++) {
            if ((page_avl[i] & (1 << j)) == 0) {
                page_avl_index = i;
                return (i * 8 + j) << 12;
            }
        }
    }

    // TODO : Add memory deallocation policies
    return -1;
}

void set_page(int address)
{
    page_avl[(address >> 12) / 8] |= (1 << ((address >> 12) % 8));
}

void clear_page(int address)
{
    page_avl[(address >> 12) / 8] |= (1 << ((address >> 12) % 8));
    page_avl[(address >> 12) / 8] ^=  (1 << ((address >> 12) % 8));
}

struct page_directory_param {
    u8 P;
    u8 R_W;
    u8 U;
    u8 PWT;
    u8 PCD;
    u8 A;
    u8 PS;
    u32 address;
};

struct page_table_param {
    u8 P;
    u8 R_W;
    u8 U;
    u8 PWT;
    u8 PCD;
    u8 A;
    u8 D;
    u8 PAT;
    u8 G;
    u32 address;
};

struct page_directory_entry create_page_directory_entry(struct page_directory_param param)
{
    struct page_directory_entry page_dir;

    page_dir.flags = 0;
    page_dir.flags |= (u8) param.P;
    page_dir.flags |= (u8) (param.R_W << 1);
    page_dir.flags |= (u8) (param.U << 2);
    page_dir.flags |= (u8) (param.PWT << 3);
    page_dir.flags |= (u8) (param.PCD << 4);
    page_dir.flags |= (u8) (param.A << 5);
    page_dir.flags |= (u8) (param.PS << 7);

    page_dir.available = (u8) 0;

    page_dir.address = (u32) (param.address >> 12);

    return page_dir;
}

struct page_table_entry create_page_table_entry(struct page_table_param param)
{
    struct page_table_entry page_table;

    page_table.flags = 0;
    page_table.flags = (u8) param.P;
    page_table.flags |= (u8) (param.R_W << 1);
    page_table.flags |= (u8) (param.U << 2);
    page_table.flags |= (u8) (param.PWT << 3);
    page_table.flags |= (u8) (param.PCD << 4);
    page_table.flags |= (u8) (param.A << 5);
    page_table.flags |= (u8) (param.D << 6);
    page_table.flags |= (u8) (param.PAT << 7);

    page_table.g = (u8) param.G;

    page_table.available = (u8) 0;

    page_table.address = (u32) param.address;

    return page_table;
}

int create_kernel_page(void)
{
    page_dir = (struct page_directory_entry *) KERNEL_PAGE_DIR;
    page_table = (struct page_table_entry *) KERNEL_PAGE_TABLE;

    for (int i = 0; i < NB_KERNEL_PAGE_DIR; i++)
    {
        for (int j = 0; j < 1024; j++)
        {
            page_table[i * 1024 + j] = create_page_table_entry((struct page_table_param) {
                        .P = 1, .R_W = 1, .U = 0, .PWT = 0, .PCD = 0,
                        .A = 0, .D = 0, .PAT = 0, .G = 0,
                        .address = j + i * 1024});
        }
        DEBUG_INFO("PAGE_TABLE %d", &page_table[i * 1024]);
        page_dir[i] = create_page_directory_entry((struct page_directory_param) {
                    .P = 1, .R_W = 1, .U = 0, .PWT = 0, .PCD = 0,
                    .A = 0, .PS = 0, .address = &page_table[i * 1024]});
    }

    for (int i = NB_KERNEL_PAGE_DIR; i < 1024; i++)
    {
        page_dir[i] = create_page_directory_entry((struct page_directory_param) {
                        .P = 0, .R_W = 0, .U = 0, .PWT = 0, .PCD = 0,
                        .A = 0, .PS = 0, .address = 0});
    }

    // #define DEBUG_PAGING
    #ifdef DEBUG_PAGING
    DEBUG_INFO("------ page_dir ------");
    for (int i = 0; i < 1024; i++)
    {   
        int val = ((int *) page_dir)[i];
        if (i % 64 == 63)
        {
            write_serial_nb(val, false);
            write_serial("\n");
        }
        else
        {
            write_serial_nb(val, false);
            write_serial(" ");
        }
    }

    DEBUG_INFO("------ page_table ------");
    for (int i = 0; i < 1024; i++)
    {
        u32 val = ((u32 *) page_table)[i];
        if (i % 64 == 63)
        {
            write_serial_nb(val, false);
            write_serial("\n");
        }
        else
        {
            write_serial_nb(val, false);
            write_serial(" ");
        }
    }
    #endif

    DEBUG_INFO("address of Page Directory Array: %d", page_dir);
    DEBUG_INFO("address of the first Page Table array: %d", page_table);

    // load page directory and enable paging (cr0 bit 31)
    asm volatile ("            \
    mov %0, %%eax           \n \
    mov %%eax, %%cr3        \n \
    mov %%cr0, %%eax        \n \
    or $0x80000000, %%eax   \n \
    mov %%eax, %%cr0" : "=m" (page_dir));

    return 0;
}

int create_new_userland_page(int uid)
{
    struct page_directory_entry *userland_page_dir = userland_data->userland_data[uid].page_directories;
    struct page_table_entry *kernel_page_table = (struct page_table_entry *) KERNEL_PAGE_TABLE;

    for (int i = 0; i < NB_KERNEL_PAGE_DIR; i++)
    {
        DEBUG_INFO("PAGE_TABLE %d", &kernel_page_table[i * 1024]);
        userland_page_dir[i] = create_page_directory_entry((struct page_directory_param) {
                        .P = 1, .R_W = 1, .U = 0, .PWT = 0, .PCD = 0,
                        .A = 0, .PS = 0, .address = &kernel_page_table[i * 1024]});
    }

    for (int i = 0; i < 1024; i++)
    {
        int avl_address = find_page_avl();
        set_page(avl_address);
        userland_data->userland_data[uid].page_table[i] = create_page_table_entry((struct page_table_param) {
                        .P = 1, .R_W = 1, .U = 0, .PWT = 0, .PCD = 0,
                        .A = 0, .D = 0, .PAT = 0, .G = 0,
                        .address = (avl_address >> 12)});
    }

    userland_page_dir[NB_KERNEL_PAGE_DIR] = create_page_directory_entry((struct page_directory_param) {
                        .P = 1, .R_W = 1, .U = 0, .PWT = 0, .PCD = 0,
                        .A = 0, .PS = 0, .address = userland_data->userland_data[uid].page_table});

    for (int i = NB_KERNEL_PAGE_DIR + 1; i < 1024; i++)
    {
        userland_page_dir[i] = create_page_directory_entry((struct page_directory_param) {
                        .P = 0, .R_W = 0, .U = 0, .PWT = 0, .PCD = 0,
                        .A = 0, .PS = 0, .address = 0});
    }
}

int allocate_new_page(int uid, int address)
{
    struct page_directory_entry *userland_page_dir = userland_data->userland_data[uid].page_directories;

    int dir_address = address >> 22;
    int table_address = (address >> 12) ^ ((address >> 22) << 10);

    DEBUG_INFO("dir_address %d", dir_address);
    DEBUG_INFO("table_address %d", table_address);

    // CPU does't use page table when decoding page table, so it need the physical address
    int new_page_table_real = (int) userland_data->userland_data[uid].page_table[dir_address].address;
    DEBUG_INFO("PAGE TABLE REAL ADDRESS %d", new_page_table_real << 12);
    struct page_table_entry *new_page_table = USERLAND_BASE_ADDRESS + dir_address * 1024 * 4 + table_address * 4;

    if (userland_page_dir[dir_address].address != 0 && new_page_table->address != 0)
        clear_page(new_page_table->address);

    if (userland_page_dir[dir_address].address == 0)
    {
        DEBUG_INFO("NEW DIR ENTRY");
        userland_page_dir[dir_address] = create_page_directory_entry((struct page_directory_param) {
                        .P = 1, .R_W = 1, .U = 1, .PWT = 0, .PCD = 0,
                        .A = 0, .PS = 0, .address = new_page_table_real << 12});
    }

    int avl_address = find_page_avl();
    set_page(avl_address);
    *new_page_table = create_page_table_entry((struct page_table_param) {
                        .P = 1, .R_W = 1, .U = 1, .PWT = 0, .PCD = 0,
                        .A = 0, .D = 0, .PAT = 0, .G = 0,
                        .address = (avl_address >> 12)});
}
