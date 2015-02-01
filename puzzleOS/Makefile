ifndef CROSS
CROSS = /usr/cross/i586-elf/bin/
endif

CC = $(CROSS)gcc
AS = $(CROSS)as
LD = $(CROSS)ld


CFLAGS = -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin

#CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector

LDFLAGS = -T linker.ld

S_SRCFILES = $(shell find ./ -name "*.s")
C_SRCFILES += $(shell find ./ -name "*.c")

OBJECTS = $(patsubst %.s,%.o,$(S_SRCFILES))
OBJECTS += $(patsubst %.c,%.o,$(C_SRCFILES))


all: kernel.elf image

kernel.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

image:
	cp kernel.elf iso/kernel.elf
	cp ../rd/initrd.img iso/initrd.img
	mkisofs -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o puzzleOS.iso iso

clean:
	rm kernel.elf puzzleOS.iso *.o
