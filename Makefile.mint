PREFIX=m68k-atari-mint
CC=$(PREFIX)-gcc-4.6.4

CFLAGS = -g -Wall -pedantic -O3 -std=c99 -mshort -mfastcall
LDFLAGS =

LIBCMINI_DIR=$(HOME)/src/libcmini/build
LINK=-nostdlib $(LIBCMINI_DIR)/crt0.o $< -L$(LIBCMINI_DIR)/mshort/mfastcall $(LDFLAGS) -lcmini -lgcc -o $@
#LINK=$< $(LDFLAGS) -o $@

bin = AUTO/8080TEST.PRG
src = $(wildcard *.c)
obj = $(src:.c=.o)

.PHONY: all clean

all: $(bin)

$(bin): i8080_tests.c
	$(CC) $(CFLAGS) $(LINK)
	$(PREFIX)-objdump -drwC $@ > $@.s
	@$(PREFIX)-size -A $@
	@du -b $@

clean:
	-rm $(bin) $(obj)

.PHONY: test

test: $(bin)
	flatpak run org.tuxfamily.hatari --harddrive . --frameskips 4 --mono
