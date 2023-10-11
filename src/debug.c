#include "debug.h"

#include <stdarg.h>

#include "serial.h"

int print_variadic(char *msg, va_list args)
{
    while (*msg != '\0')
    {
        if (*(msg + 1) != '\0' && *msg == '%')
        {
            switch (*(msg + 1))
            {
            case 'd':
                write_serial_nb(va_arg(args, int), false);
                break;
            case 's':
                write_serial(va_arg(args, char *));
                break;
            case 'b':
                write_serial_bin((int) va_arg(args, char *), false);
                break;
            case 'c':
                write_serial_char(va_arg(args, int));
                break;
            default:
                write_serial("?");
                break;
            }
            msg += 2;
        }
        else
        {
            write_serial_char(*msg);
            msg++;
        }
    }

    return 0;
}

int debug_info(const char *fnt, char *msg, ...)
{
    write_serial("\033[0;34m[INFO]\033[0m     ");
    write_serial((char *) fnt);
    write_serial("\t: ");

    va_list args;
    va_start(args, msg);
    print_variadic(msg, args);
    va_end(args);

    write_serial("\n");

    return 0;
}

int debug_warn(const char *fnt, char *msg, ...)
{
    write_serial("\033[0;33m[WARNING]\033[0m  ");
    write_serial((char *) fnt);
    write_serial("\t: ");
    
    va_list args;
    va_start(args, msg);
    print_variadic(msg, args);
    va_end(args);

    write_serial("\n");

    return 0;
}

int debug_err(const char *fnt, char *msg, ...)
{
    write_serial("\033[0;31m[ERROR]\033[0m    ");
    write_serial((char *) fnt);
    write_serial("\t: ");

    va_list args;
    va_start(args, msg);
    print_variadic(msg, args);
    va_end(args);

    write_serial("\n");

    return 0;
}