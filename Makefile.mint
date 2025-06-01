PREFIX=m68k-atari-mint
CC=$(PREFIX)-gcc

CFLAGS = -g -Wall -pedantic -O3
LDFLAGS =

bin = AUTO/8080TEST.PRG
src = $(wildcard *.c)
obj = $(src:.c=.o)

.PHONY: all clean

all: $(bin)

$(bin): i8080_tests.c
	$(CC) -o $@ $^ $(LDFLAGS)
	$(PREFIX)-objdump -drwC $@ > $@.s
	@$(PREFIX)-size -A $@

clean:
	-rm $(bin) $(obj)

.PHONY: test

test: $(bin)
	flatpak run org.tuxfamily.hatari --harddrive . --frameskips 4 --mono
