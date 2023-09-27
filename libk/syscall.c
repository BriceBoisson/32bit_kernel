#include "syscall.h"

int __syscall(int num, void *a0, void *a1, void *a2, void *a3, void *a4)
{
    int ret;
    asm volatile("      \n \
        mov %1, %%eax;  \n \
        mov %2, %%ebx;  \n \
        mov %3, %%ecx;  \n \
        mov %4, %%edx;  \n \
        mov %5, %%edi;  \n \
        mov %6, %%esi;  \n \
        int $48;        \n \
        mov %%eax, %0" : "=m" (ret),
                         "+m" (num),
                         "+m" (a0),
                         "+m" (a1),
                         "+m" (a2),
                         "+m" (a3),
                         "+m" (a4));
    return ret;
}

int write(int fd, char *buf, int count)
{
    return __syscall(1, (void *) fd, (void *) buf, (void *) count, (void *) 0, (void *) 0);
}
