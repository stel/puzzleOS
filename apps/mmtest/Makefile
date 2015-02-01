ifndef CROSS
CROSS = /usr/cross/i586-elf/bin/
endif

CC = $(CROSS)gcc
AS = $(CROSS)as
LD = $(CROSS)ld


CFLAGS = -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I../../puzzlestd/



all:
	$(CC) $(CFLAGS) -c -o mmtest.o mmtest.c
	$(LD) --oformat binary -Ttext 0x400000 -o mmtest.bin ../../puzzlestd/startup.o  ../../puzzlestd/puzzlestd.o ../../puzzlestd/vsprintf.o ../../puzzlestd/string.o mmtest.o
	cp mmtest.bin ../../rd/mmtest.bin
	
	
mmtest.bin: mmtest.o

clean:
	rm -f *.bin *.o
