C = gcc
CFLAGS = -m32 -O2 -Wall -Werror -fno-builtin
CFLAGS += -I./include

ASFLAGS = -m32

CSRC = $(shell find src/ -name "*.c")

OBJS = $(CSRC:.c=.o)

loader: $(OBJS) src/start.o
	ld -o loader -e start -Ttext 0x100000 -m elf_i386 src/start.o $(OBJS)

clean:
	-rm -f loader $(OBJS) src/start.o 2> /dev/null

