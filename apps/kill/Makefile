ifndef CROSS
CROSS = /usr/cross/i586-elf/bin/
endif

CC = $(CROSS)gcc
AS = $(CROSS)as
LD = $(CROSS)ld


CFLAGS = -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I../../puzzlestd/



all:
	$(CC) $(CFLAGS) -c -o kill.o kill.c
	$(LD) --oformat binary -Ttext 0x400000 -o kill.bin ../../puzzlestd/startup.o  ../../puzzlestd/puzzlestd.o ../../puzzlestd/vsprintf.o ../../puzzlestd/string.o kill.o
	cp kill.bin ../../rd/kill.bin
	
	
kill.bin: kill.o

clean:
	rm -f *.bin *.o
