#include <types.h>
#include "io.h"
#include "serial.h"
#include "pic_controler.h"
#include "debug.h"
#include "syscall.h"

void isr_default_int(void)
{
    // DEBUG_INFO("An INT has been raised, entering default interrupt handler.");
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

int syscall_handler(int eax, void *ebx, void *ecx, void *edx, void *edi, void *esi)
{
    DEBUG_INFO("Syscall %d has been called from the userland with parameters: %d, %d, %d, %d, %d", eax, ebx, ecx, edx, edi, esi);

    switch (eax)
    {
    case 1:
        int fd = (int) ebx;
        char *buf = (char *) ecx;
        int size = (int) edx;
        return write(fd, buf, size);
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
