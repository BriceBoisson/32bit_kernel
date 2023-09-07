# Introduction

This kernel is written as a educative project. The goal is to have a kernel able to run a basic shell. To allow that, this kernel have to implement :
- [X] Protected mode (loading gdt)
- [X] Manage interrupt (loading idt and enabling interrupt)
- [X] Manage IRQ (enable PIC)
- [X] Running and Userland (TSS)
- [X] Let the userland call the kernel (syscall)
- [ ] Use paging
- [ ] Running multiple userland (ordonnanceur)
- [ ] Using ext2 File System
- [ ] Running elf binary
- [ ] Having a small libc and needed syscall to run the shell
- [ ] (Bonus) Having a basic visual interface (VGA ?)

This kernel has been written using :
- The tutorial Pépin OS - Réaliser son propre système (in french): https://michelizza.developpez.com/realiser-son-propre-systeme
- The educative k project from former EPITA (Engineering School) system laboratory LSE:
https://k.lse.epita.fr/
https://github.com/Galli24/kernel-k-project
- The ressources on the website OSDev:
https://wiki.osdev.org/Main_Page
- The in Intel® 64 and IA-32 Architectures, Software Developer’s Manual, Volume 3A: System Programming Guide, Part 1:
https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html
