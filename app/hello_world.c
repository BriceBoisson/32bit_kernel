#include <syscall.h>

static char s[] = "Hello from userland!\n";

int main(void)
{
    write(1, s, 21);
    while (1);
    return 0;
}
