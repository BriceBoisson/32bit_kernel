include Makefile.common

all: bin k.iso

k.iso: install
	./tools/create_iso.sh

install:
	mkdir -p $(GRUBDIR)
	$(MAKE) -C $(SOURCEDIR) $@

bin:
	$(MAKE) -C $(LIB)
	gcc -c test.c -Ilibk -Llibk -lk -m32
	ld -m elf_i386 -Ttext=0x6000000 --entry=main test.o -L./libk/ -lk
	objcopy --input binary --output elf32-i386 --binary-architecture i386 --rename-section .data=.rodata,CONTENTS,ALLOC,LOAD,READONLY,DATA a.out myfile.o
	cp myfile.o $(SOURCEDIR)/myfile.o

clean:
	$(MAKE) -C $(SOURCEDIR) $@
	$(MAKE) -C $(LIB) $@
	$(RM) kernel.iso
	$(RM) -r iso

.PHONY: $(GRUBDIR) install
