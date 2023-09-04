include Makefile.common

all: k.iso

k.iso: install
	./tools/create_iso.sh

install:
	mkdir -p $(GRUBDIR)
	$(MAKE) -C $(SOURCEDIR) $@

clean:
	$(MAKE) -C $(SOURCEDIR) $@
	$(RM) kernel.iso
	$(RM) -r iso

.PHONY: $(GRUBDIR) install
