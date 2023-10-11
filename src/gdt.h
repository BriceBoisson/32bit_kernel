#ifndef GDT_H
#define GDT_H

#include <types.h>

struct gdtr {
        u16     limit;
        u32     base;
} __attribute__((packed));

struct segdesc {
       u16      Limit_1;
       u16      Base_1;
       u8       Base_2;
       u8       Type    : 4;
       u8       Flag_1  : 4;
       u8       Limit_2 : 4;
       u8       Flag_2  : 4;
       u8       Base_3;
} __attribute__((packed));

#ifdef __GDT__
        static struct segdesc gdt[6];
        struct gdtr kgdtr;
#endif

void init_gdt(void);
void temp_run_process(void);

#endif /*       !GDT_H */