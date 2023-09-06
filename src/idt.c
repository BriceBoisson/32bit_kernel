#define __IDT__

#include "idt.h"
#include <types.h>
#include "serial.h"
#include "pic_controler.h"
#include "debug.h"
#include "int.h"

/*
    Make space for the interrupt descriptor table
    Tell the CPU where that space is (see GDT Tutorial: lidt works the very same way as lgdt)
    Tell the PIC that you no longer want to use the BIOS defaults (see Programming the PIC chips)
    Write a couple of ISR handlers (see Interrupt Service Routines) for both IRQs and exceptions
    Put the addresses of the ISR handlers in the appropriate descriptors (in Interrupt Descriptor Table)
    Enable all supported interrupts in the IRQ mask (of the PIC)
*/

struct interrupt_gate_param {
    u32     Offset;
    u16     SegSelect;
    u8      Type;
    u8      D;
    u8      DPL;
    u8      P;
};

struct idt_segdesc init_gate(struct interrupt_gate_param param)
{
    if (param.Type == 5)
    {
        param.Offset = 0;
        param.D = 0;
        param.Offset = 0;
    }

    struct idt_segdesc descriptor;

    descriptor.Offset       =       param.Offset;
    descriptor.Offset2      =       param.Offset >> 16;

    descriptor.SegSelect    =       param.SegSelect;

    descriptor.empty        =       0;
    descriptor.bits         =       0;

    descriptor.Type         =       param.Type | (param.D << 3);

    descriptor.Flags        |=       param.DPL << 1;
    descriptor.Flags        |=       param.P << 3;

    return descriptor;
}

void init_idt(void)
{
    DEBUG_INFO("Initializing IDT");

    DEBUG_INFO("IDT BASE ADDRESS: %d", (u32) &kidtr);
    DEBUG_INFO("IDT LIMIT: %d", sizeof(idt) - 1);
    kidtr.limit     = sizeof(idt) - 1;
    kidtr.base      = (u32) idt;
    
    for (int i = 0; i < sizeof(idt); i++)
    {
        idt[i] = init_gate((struct interrupt_gate_param) { .Offset = (u32) _asm_default_int, .SegSelect = 0x08,
        .Type = 0x06, .D = 1, .DPL = 0, .P = 1 });
    }

    idt[32] = init_gate((struct interrupt_gate_param) { .Offset = (u32) _asm_irq_0, .SegSelect = 0x08,
        .Type = 0x06, .D = 1, .DPL = 0, .P = 1 });
    
    idt[33] = init_gate((struct interrupt_gate_param) { .Offset = (u32) _asm_irq_1, .SegSelect = 0x08,
        .Type = 0x06, .D = 1, .DPL = 0, .P = 1 });
    
    idt[48] = init_gate((struct interrupt_gate_param) { .Offset = (u32) _asm_sycall_handler, .SegSelect = 0x08,
        .Type = 0x07, .D = 1, .DPL = 3, .P = 1 });

    #ifdef DEBUG_IDT
    for (int i = 0; i < 49; i++)
    {
        char *ptr = (char *) &idt[i];
        DEBUG_INFO("--------------------");
        DEBUG_INFO("IDT[%d]:", i);
        DEBUG_INFO("\tOffset            : %d", (u32) ptr[0] | (ptr[6] << 16));
        DEBUG_INFO("\tSegment Selector  : %d | %b | %b |Index|GDT 0 / LDT 1|Level ring 0 / ring 1|", (u16) ptr[2] >> 3, ptr[2] >> 2, ptr[2]);
        DEBUG_INFO("\tZeroes            : %d", (u8) ptr[4]);
        DEBUG_INFO("\tFlag              : %b |16 bits 0 / 32 bits 1|Task 101 / Interrupt 110 / Trap 111|", (u8) ptr[5] & 0x0F);
        DEBUG_INFO("\tFlag              : %b |Present|Level ring 0 / ring 1|Zero|", (u8) ptr[5] >> 4);
    }
    DEBUG_INFO("--------------------");
    #endif

    asm volatile ("lidtl (kidtr)");

    DEBUG_INFO("IDT LOADED");

    DEBUG_INFO("ENABLING STI");
}
