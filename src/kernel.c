#include <types.h>

#include "gdt.h"
#include "idt.h"
#include "pic_controler.h"
#include "serial.h"
#include "debug.h"
#include "tss.h"
#include "launch_process.h"
#include "elf.h"
#include "userland.h"

extern char _binary_a_out_start[];
extern char _binary_a_out_end[];
extern char _binary_a_out_size[];

void main(void)
{
	DEBUG_INFO("Entering Main Function");

	char *data_start	= (char *) &_binary_a_out_start;

	create_process(0, data_start);
	switch_to_process(0);

	for (;;)
	{
		DEBUG_INFO("Waiting for an interrupt");
		asm volatile ("hlt");
	}
}

void kernel_main(void)
{
	init_serial();

	DEBUG_INFO("Starting kernel");

	init_gdt();
	init_idt();
	pic_init();
	create_kernel_page();
	asm volatile ("sti");

	main();
}
