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

	char *data_start     = &_binary_a_out_start;
    char *data_end       = _binary_a_out_end;
    size_t data_size  = (size_t)_binary_a_out_size;


	create_new_userland_page(0);

	DEBUG_INFO("PAGE DIR: %d", userland_data->userland_data[0].page_directories);
	struct page_table_entry *ttt = userland_data->userland_data[0].page_directories[20].address << 12;
	DEBUG_INFO("PAGE DIR: %d", ttt->address);

    asm volatile ("            \
    mov $0x3020000, %eax     \n \
    mov %eax, %cr3");

	// DEBUG_INFO("BIN %b", 0x13000000);
	allocate_new_page(0, 0x32400000);

	// asm volatile ("            \
    // xor $0x80000000, %eax   \n \
    // mov %eax, %cr0");

	// u32 *data2 = 50466816;
	// DEBUG_INFO("BIN %b", *data2);

	// u32 *data = 83886080;
	// DEBUG_INFO("BIN %b", *data);

	// struct page_directory_entry *var = 0x3020000;
	// DEBUG_INFO("address %d", var[76].address);

	// u32 *var = 0x13000000;
	u32 *var = 0x32403400;
	*var = 10;
	DEBUG_INFO("%d", *var);
    // asm volatile ("            \
    // mov %0, %%eax           \n \
    // mov %%eax, %%cr3" : "=m" (userland_data->userland_data[0].page_directories));

	// load_elf(data_start);

	// launch_process(0x28, 0x30000, 0x20, 0x18, 0x20);

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
