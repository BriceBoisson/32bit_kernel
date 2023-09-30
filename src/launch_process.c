#include "launch_process.h"

#include "tss.h"
#include "debug.h"
#include "userland.h"

void launch_process(u16 tss, int memory_start, int userland_stack, int userland_code, u16 userland_data_seg)
{
    // Setting DPL and GDT bits
    userland_stack += 3;
    userland_code += 3;
    userland_data_seg += 3;

    DEBUG_INFO("LAUCHING USER LAND PROCESS");

    asm volatile ("         \n \
        mov %0, %%ax	    \n \
        ltr %%ax            \n \
        movw %%ss, %1       \n \
        movl %%esp, %2      \n \
        movw $0x23, %%ax    \n \
        movw %%ax, %%ds     \n \
        movw %%ax, %%es     \n \
        movw %%ax, %%fs     \n \
        movw %%ax, %%gs     \n \
        mov %%esp, %%eax    \n \
        push $0x23          \n \
        push $0x7FFFEFFF    \n \
        pushfl              \n \
        push $0x1B          \n \
        push $0x6000000     \n \
        iret" : "+r" (tss),
                "=m" (user_land_tss.ss0),
                "=m" (user_land_tss.esp0));
}
