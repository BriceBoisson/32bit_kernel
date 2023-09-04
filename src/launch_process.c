#include "launch_process.h"

#include "tss.h"
#include "debug.h"

void launch_process(int tss, int memory_start, int userland_stack, int userland_code, int userland_data)
{
    // Setting DPL and GDT bits
    userland_stack += 3;
    userland_code += 3;
    userland_data += 3;

    DEBUG_INFO("LAUCHING USER LAND PROCESS");

    asm volatile ("             \n \
        movw %0, %%ax           \n \
        ltr %%ax                \n \
        movw %%ss, %1           \n \
        movl %%esp, %2          \n \
        cli                     \n \
        push %3                 \n \
        push %4                 \n \
        pushfl                  \n \
        popl %%eax              \n \
        orl $0x200, %%eax       \n \
        and $0xffffbfff, %%eax  \n \
        push %%eax              \n \
        push %5                 \n \
        push $0x0               \n \
        movl $0x20000, %6       \n \
        movw %7, %%ax           \n \
        movw %%ax, %%ds         \n \
        iret" : "=m" (tss),
                "=m" (user_land_tss.ss0),
                "=m" (user_land_tss.esp0),
                "=m" (userland_stack),
                "=m" (memory_start),
                "=m" (userland_code), 
                "=m" (user_land_tss.esp0),
                "=m" (userland_data));
}
