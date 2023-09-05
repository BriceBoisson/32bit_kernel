#include "paging.h"

struct page_directory_entry page_dir[1024];
struct page_table_entry page_table[1024];

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

    page_dir.flags = 2;

    page_dir.available = (u8) 10;

    page_dir.address = (u32) 24;

    return page_dir;
}

struct page_table_entry create_page_table_entry(struct page_table_param param)
{
    struct page_table_entry page_table;

    page_table.flags = 2;

    page_table.g = (u8) 10;

    page_table.available = (u8) 0;

    page_table.address = (u32) 24;

    return page_table;
}

int make_page(void)
{
    page_table[0] = create_page_table_entry((struct page_table_param) {
                .P = 1, .R_W = 1, .U = 1, .PWT = 1, .PCD = 1,
                .A = 0, .D = 0, .PAT = 0, .G = 0,
                .address = 0});
    
    for (int i = 1; i < 1024; i++)
    {
        page_table[i] = create_page_table_entry((struct page_table_param) {
                .P = 0, .R_W = 0, .U = 0, .PWT = 0, .PCD = 0,
                .A = 0, .D = 0, .PAT = 0, .G = 0,
                .address = 0});
    }

    page_dir[0] = create_page_directory_entry((struct page_directory_param) {
                .P = 1, .R_W = 1, .U = 1, .PWT = 1, .PCD = 1,
                .A = 0, .PS = 0, .address = page_table});
    for (int i = 1; i < 1024; i++)
    {
        page_dir[i] = create_page_directory_entry((struct page_directory_param) {
                        .P = 0, .R_W = 0, .U = 0, .PWT = 0, .PCD = 0,
                        .A = 0, .PS = 0, .address = 0});
    }

    return 0;
}
