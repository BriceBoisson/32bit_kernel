#ifndef TSS_H
#define TSS_H

#include <types.h>

struct tss
{
    u16 previous_task;
    u16 reserved_1;
    u32 esp0;
    u16 ss0;
    u16 reserved_2;
    u32 esp1;
    u16 ss1;
    u16 reserved_3;
    u32 esp2;
    u16 ss2;
    u16 reserved_4;
    u32 cr3;
    u32 eip;
    u32 eflags;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esp;
    u32 ebp;
    u32 esi;
    u32 edi;
    u16 es;
    u16 reserved_5;
    u16 cs;
    u16 reserved_6;
    u16 ss;
    u16 reserved_7;
    u16 ds;
    u16 reserved_8;
    u16 fs;
    u16 reserved_9;
    u16 gs;
    u16 reserved_10;
    u16 lst_seg_select;
    u16 reserved_11;
    u16 t_bitmap : 1;
    u16 reserved_12 : 15;
    u16 io_map_base_address;
    u32 ssp;
} __attribute__((packed));

extern struct tss user_land_tss;

#endif /*   !TSS_H */
