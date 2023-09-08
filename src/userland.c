#include "userland.h"

// void userland(void)
// {
//     int res = 0;
//     // asm ("mov $1, %0" : "=r" (res));
//     // asm volatile ("movl $2, %eax; int $0x30");
//     // asm("movl %%eax,%0" : "=r"(res));
//     // asm volatile ("int $0x30");
//     char *str = (void *) 0x30100;
//     str[0] = 'H';
//     str[1] = 'e';
//     str[2] = 'l';
//     str[3] = 'l';
//     str[4] = 'o';
//     str[5] = '\0';
//     asm volatile ("mov $1, %%eax; movl $0x30100, %%ebx; int $0x30; movl %%eax, %1" : "=m" (str), "=r" (res));
//     // asm volatile ("mov $1, %%eax; movl %0, %%ebx; int $0x30" : "=m" (str));
//     // asm ("mov $1, %eax; int $0x30");
//     // asm ("movl %0, %eax; int $0x30" : "=m" (res));
//     // asm ("movl %eax, %eax; int $0x30");
//     // asm ("movl $28, %eax; movl $5, %ebx; int $0x30");
//     // asm ("movl $43, %eax; movl $7, %ebx; int $0x30");
//     while (1);
//     return; /* never go there */
// }