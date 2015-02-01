ifndef CROSS
CROSS = /usr/cross/i586-elf/bin/
endif

CC = $(CROSS)gcc
AS = $(CROSS)as
LD = $(CROSS)ld


CFLAGS = -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I../../puzzlestd/



all:
	$(CC) $(CFLAGS) -c -o hello.o hello.c
	$(LD) --oformat binary -Ttext 0x400000 -o hello.bin ../../puzzlestd/startup.o  ../../puzzlestd/puzzlestd.o ../../puzzlestd/vsprintf.o ../../puzzlestd/string.o hello.o
	cp hello.bin ../../rd/hello.bin
	
	
hello.bin: hello.o

clean:
	rm -f *.bin *.o
