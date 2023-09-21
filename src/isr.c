#include <types.h>
#include "io.h"
#include "serial.h"
#include "pic_controler.h"
#include "debug.h"
#include "syscall.h"

void isr_default_int(void)
{
    DEBUG_INFO("An INT has been raised, entering default interrupt handler.");
}

void isr_page_fault(void)
{
    DEBUG_INFO("A page fault exception occured");
}

void isr_clock_int(void)
{
    DEBUG_INFO("Enterring clock interrupt handler.");
}

void isr_kbd_int(void)
{
    DEBUG_INFO("Enterring keyboard interrupt handler.");

    int x = inb(0x60);
    DEBUG_INFO("Keyboard input: %d", x);
}

void syscall_handler(int eax, int ebx)
{
    DEBUG_INFO("Syscall %d has been called from the userland with parameter %d", eax, ebx);

    switch (eax)
    {
    case 1:
        char t = *((char *) ebx);
        DEBUG_INFO("Syscall write : %d", (int) t);
        return write(1, (u32) ebx);
        break;
    case 2:
        DEBUG_INFO("Syscall keyboard");
        return keyboard();
        break;
    default:
        DEBUG_INFO("No syscall %d", eax);
        break;
    }

    return 0;
}
