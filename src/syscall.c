#include "syscall.h"
#include "debug.h"
#include "serial.h"

/*
 *  Syscall handler for write, currently only serial write is supported.
 *  Use the fd 1, to make a serial write.
 */
int write(int fd, void *buf, int size)
{
    if (fd != 1 || size < 0)
        return -1;

    if (write_serial(buf))
        return -1;

    return 0;
}

/*
 *  Syscall handler for keyboard input, this function is currently not implemented.
 */
int keyboard(void)
{
    return 1;
}
