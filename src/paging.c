#include "paging.h"

#include "debug.h"
#include "serial.h"

struct page_directory_entry *page_dir = 0x2000; //__attribute__((aligned(4096)));
struct page_table_entry *page_table = 0x3000; //__attribute__((aligned(4096)));

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

int make_page(void)
{
    for (int i = 0; i < 1024; i++)
    {
        page_table[i] = create_page_table_entry((struct page_table_param) {
                    .P = 1, .R_W = 1, .U = 1, .PWT = 0, .PCD = 0,
                    .A = 0, .D = 0, .PAT = 0, .G = 0,
                    .address = 1 * i});
    }

    page_dir[0] = create_page_directory_entry((struct page_directory_param) {
                .P = 1, .R_W = 1, .U = 1, .PWT = 0, .PCD = 0,
                .A = 0, .PS = 0, .address = page_table});

    for (int i = 1; i < 1024; i++)
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

    asm volatile ("            \
    mov %0, %%eax           \n \
    mov %%eax, %%cr3        \n \
    mov %%cr0, %%eax        \n \
    or $0x80000000, %%eax   \n \
    mov %%eax, %%cr0" : "=m" (page_dir));

    return 0;
}
