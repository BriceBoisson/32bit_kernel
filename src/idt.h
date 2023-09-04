#ifndef IDT_H
#define IDT_H

#include <types.h>

struct idtr {
        u16     limit;
        u32     base;
} __attribute__((packed));

struct idt_segdesc {
    u16      Offset;
    u16      SegSelect;
    u8       empty  : 5;
    u8       bits   : 3;
    u8       Type   : 4;
    u8       Flags  : 4;
    u16      Offset2;
} __attribute__((packed));

#ifdef __IDT__
    static struct idt_segdesc idt[256];
    struct idtr kidtr;
#endif

void init_idt(void);

#endif				/* !IDT_H */