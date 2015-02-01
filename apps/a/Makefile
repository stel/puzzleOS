ifndef CROSS
CROSS = /usr/cross/i586-elf/bin/
endif

CC = $(CROSS)gcc
AS = $(CROSS)as
LD = $(CROSS)ld


CFLAGS = -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I../../puzzlestd/



all:
	$(CC) $(CFLAGS) -c -o a.o a.c
	$(LD) --oformat binary -Ttext 0x400000 -o a.bin ../../puzzlestd/startup.o ../../puzzlestd/string.o ../../puzzlestd/vsprintf.o ../../puzzlestd/puzzlestd.o a.o
	cp a.bin ../../rd/a.bin
	
	
a.bin: a.o

clean:
	rm -f *.bin *.o
