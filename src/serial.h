#ifndef SERIAL_H
#define SERIAL_H

#include <types.h>

int init_serial();
int write_serial(char *s);
int write_serial_nb(int nb, bool ln);
int write_serial_bin(int nb, int ln);
int write_serial_char(char c);

#endif /* SERIAL_H */