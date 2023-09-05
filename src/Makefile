include ../Makefile.common

OBJS	= \
	  boot.o \
	  kernel.o \
	  serial.o \
	  isr.o \
	  debug.o \
	  syscall.o \
	  int.o \
	  gdt.o \
	  idt.o \
	  pic_controler.o \
	  userland.o \
	  launch_process.o \
	  paging.o \

DEPS = $(OBJS:.o=.d)

CPPFLAGS += -MMD -Iinclude
CFLAGS += $(K_EXTRA_CFLAGS) -g -nostdlib
LDFLAGS += -Wl,-Tkernel.lds
LDLIBS =

all: $(TARGET) $(OBJS)

$(TARGET): $(OBJS)

install: $(TARGET)
	install $(TARGET) ../$(GRUBDIR)/$(TARGET)

clean:
	$(RM) $(OBJS) $(DEPS) $(TARGET)
