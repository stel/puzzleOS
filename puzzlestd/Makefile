ifndef CROSS
CROSS = /usr/cross/i586-elf/bin/
endif

CC = $(CROSS)gcc
AS = $(CROSS)as
LD = $(CROSS)ld

CFLAGS = -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin

all: puzzlestd.o startup.o string.o vsprintf.o
	
clean:
	rm *.o