#include "serial.h"

#include "io.h"

#define PORT 0x3f8          // COM1

int strlen(char *str)
{
	int i = 0;
	while (str[i++] != '\0');
	return i;
}

int init_serial()
{
	outb(PORT + 1, 0x00);    // Disable all interrupts
	outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outb(PORT + 1, 0x00);    //                  (hi byte)
	outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
	outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int write_serial_nb(int nb, int ln)
{
	if (nb < 10)
	{
		while ((inb(PORT + 5) & 0x20) == 0);
		outb(PORT, nb + '0');
		return 0;
	}
	
	write_serial_nb(nb / 10, false);
	
	while ((inb(PORT + 5) & 0x20) == 0);
	outb(PORT, nb % 10 + '0');

	if (ln)
	{
		while ((inb(PORT + 5) & 0x20) == 0);
		outb(PORT, '\n');
	}

	return 0;
}

int write_serial_bin(int nb, int ln)
{
	if (nb < 2)
	{
		while ((inb(PORT + 5) & 0x20) == 0);
		outb(PORT, nb + '0');
		return 0;
	}
	
	write_serial_bin(nb / 2, false);
	
	while ((inb(PORT + 5) & 0x20) == 0);
	outb(PORT, nb % 2 + '0');

	if (ln)
	{
		while ((inb(PORT + 5) & 0x20) == 0);
		outb(PORT, '\n');
	}

	return 0;
}

int write_serial_char(char c)
{
	while ((inb(PORT + 5) & 0x20) == 0);
	outb(PORT, c);

    return 0;
}

int write_serial(char * s)
{
	for (size_t i = 0; i < strlen(s); i++)
	{
		while ((inb(PORT + 5) & 0x20) == 0);
		outb(PORT, s[i]);
	}

    return 0;
}