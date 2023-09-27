include ../Makefile.common


OBJS	= \
	  syscall.o \

DEPS = $(OBJS:.o=.d)

CPPFLAGS += -MMD -Iinclude
CFLAGS += $(K_EXTRA_CFLAGS) -g -nostdlib
LDFLAGS += -Wl,-Tkernel.lds
LDLIBS =

all: $(TARGET)
	ar -rcs $(LIB).a *.o

$(TARGET): $(OBJS)

clean:
	$(RM) $(OBJS) $(DEPS) $(TARGET) $(LIB).a