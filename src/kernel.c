#include <types.h>

#include "gdt.h"
#include "idt.h"
#include "pic_controler.h"
#include "serial.h"
#include "debug.h"
#include "tss.h"
#include "launch_process.h"

void main(void)
{
	DEBUG_INFO("Entering Main Function");	

	launch_process(0x28, 0x30000, 0x20, 0x18, 0x20);

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

	main();
}
