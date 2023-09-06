#define __GDT__
#define __TSS__

#include "gdt.h"
#include <types.h>
#include "serial.h"
#include "debug.h"
#include "tss.h"
#include "userland.h"

void *memcpy(void *dest, const void *src, size_t n)
{
	const char *s = src;
	char *d = dest;

	for (size_t i = 0; i < n; i++)
		*d++ = *s++;

	return dest;
}

struct tss user_land_tss;

struct segment_desc_param {
    u16 Limit_1;
    u32 Base;
    u8  Type;
    u8  S;
    u8  DPL;
    u8  P;
    u8  Limit_2;
    u8  AVL;
    u8  L;
    u8  D_B;
    u8  G;
};

struct segdesc init_descriptor(struct segment_desc_param param)
{
    struct segdesc descriptor;
    
    descriptor.Limit_1      =       param.Limit_1;
    descriptor.Limit_2      =       param.Limit_2;

    descriptor.Base_1       = (u16) param.Base;
    descriptor.Base_2       = (u8)  (param.Base >> 16);
    descriptor.Base_3       = (u8)  (param.Base >> 24);

    descriptor.Type         =       (param.Type & 0x000F);

    descriptor.Flag_1       =       param.S;
    descriptor.Flag_1       |=      (param.DPL << 1);
    descriptor.Flag_1       |=      (param.P << 3);

    descriptor.Flag_2       =       param.AVL;
    descriptor.Flag_2       |=      (param.L << 1);
    descriptor.Flag_2       |=      (param.D_B << 2);
    descriptor.Flag_2       |=      (param.G << 3);

    return descriptor;
}

void init_gdt(void)
{
    DEBUG_INFO("Initializing GDT");

    DEBUG_INFO("GDT BASE ADDRESS: %d", (u32) &kgdtr);
    DEBUG_INFO("GDT LIMIT: %d", sizeof(gdt) - 1);
    kgdtr.limit     = sizeof(gdt) - 1;
    kgdtr.base      = (u32) gdt;

    memcpy((char*) 0x30000, &userland, 1000);

    gdt[0] = init_descriptor((struct segment_desc_param) { .Limit_1 = 0,
                            .Base = 0, .Type = 0, .S = 0, .DPL = 0, .P = 0,
                            .Limit_2 = 0, .AVL = 0, .L = 0, .D_B = 0, .G = 0 });
    
    // Code
    gdt[1] = init_descriptor((struct segment_desc_param) { .Limit_1 = 0xFFFF,
                            .Base = 0, .Type = 0x0B, .S = 1, .DPL = 0, .P = 1,
                            .Limit_2 = 0x0F, .AVL = 0, .L = 0, .D_B = 1, .G = 1 });
    // Data
    gdt[2] = init_descriptor((struct segment_desc_param) { .Limit_1 = 0xFFFF,
                            .Base = 0, .Type = 2, .S = 1, .DPL = 0, .P = 1,
                            .Limit_2 = 0x0F, .AVL = 0, .L = 0, .D_B = 1, .G = 1 });
    
    // Code
    gdt[3] = init_descriptor((struct segment_desc_param) { .Limit_1 = 0xFFFF,
                            .Base = 0, .Type = 0x0B, .S = 1, .DPL = 3, .P = 1,
                            .Limit_2 = 0x0F, .AVL = 0, .L = 0, .D_B = 1, .G = 1 });
    
    // Data
    gdt[4] = init_descriptor((struct segment_desc_param) { .Limit_1 = 0xFFFF,
                            .Base = 0, .Type = 2, .S = 1, .DPL = 3, .P = 1,
                            .Limit_2 = 0x0F, .AVL = 0, .L = 0, .D_B = 1, .G = 1 });
    
    user_land_tss.ssp = 0x0;
    user_land_tss.io_map_base_address = 0x0;
    user_land_tss.ss0 = 0x10;
    user_land_tss.esp0 = 0x20000;

    DEBUG_INFO("TSS BASE ADDRESS: %d", (u32) &user_land_tss);

    gdt[5] = init_descriptor((struct segment_desc_param) { .Limit_1 = sizeof(user_land_tss),
                            .Base = &user_land_tss, .Type = 0x09, .S = 0, .DPL = 3, .P = 1,
                            .Limit_2 = 0x00, .AVL = 0, .L = 0, .D_B = 0, .G = 0 });

    for (int i = 0; i < sizeof(gdt) / 8; i++)
    {
        char *ptr = (char *) &gdt[i];
        DEBUG_INFO("--------------------");
        DEBUG_INFO("GDT[%d] : Entry number : %d", i, i * 8);
        DEBUG_INFO("\tLIMIT : %d", (u16) ptr[0] | ((ptr[6] & 0x0F) << 16));
        // TODO : Fix display of base address
        DEBUG_INFO("\tBASE  : %d", (u32) ptr[2] | ((u32) (((u8) ptr[4]) << 16)) | ((u32) (((u8) ptr[7]) << 24)));
        DEBUG_INFO("\tTYPE  : %b |Data 0 / Code 1|Expand Down 1|Writable 1|Accessed 1|", (u8) ptr[5] & 0x0F);
        DEBUG_INFO("\tFlag  : %b |Present 1|Level ring 0 / ring 1|System 0 / Code - Data 1|", (u8) ptr[5] >> 4);
        DEBUG_INFO("\tFlag  : %b |Granularity 1|16 bits 0 / 32 bits 1|64 bits 1|Available for system 1|", (u8) ptr[6] >> 4);
    }
    DEBUG_INFO("--------------------");


    asm volatile ("lgdt (kgdtr)");

    asm volatile (
        "movw $0x10, %ax \n\
        movw %ax, %ds   \n\
        movw %ax, %es   \n\
        movw %ax, %fs   \n\
        movw %ax, %gs   \n\
        movw %ax, %ss   \n\
        ljmp $0x08, $next       \n\
        next:           \n"
    );

    DEBUG_INFO("SS0 %d", user_land_tss.ss0);
    DEBUG_INFO("ESP0 %d", user_land_tss.esp0);

    DEBUG_INFO("GDT LOADED");
}
