PREFIX=m68k-atari-mint
CC=$(PREFIX)-gcc-4.6.4

CFLAGS = -g -Wall -pedantic -O3 -std=c99 -mshort -mfastcall
CFLAGS+=-Wno-unused-value
LDFLAGS =

LIBCMINI_DIR=$(HOME)/src/libcmini/build
LINK=-nostdlib $(LIBCMINI_DIR)/crt0.o $< -L$(LIBCMINI_DIR)/mshort/mfastcall $(LDFLAGS) -lcmini -lgcc -o $@
#LINK=$< $(LDFLAGS) -o $@

TARGETS=AUTO/8080TEST.PRG SEG20.TOS
src = $(wildcard *.c)
obj = $(src:.c=.o)

.PHONY: all clean

all: $(TARGETS)

AUTO/8080TEST.PRG: i8080_tests.c
	$(CC) $(CFLAGS) $(LINK)
	$(PREFIX)-objdump -drwC $@ > $@.s
	@$(PREFIX)-size -A $@
	@du -b $@

SEG20.TOS: seg20.c
	$(CC) $(CFLAGS) $(LINK)
	$(PREFIX)-objdump -drwC $@ > $@.s
	@$(PREFIX)-size -A $@
	@du -b $@

seg20.s: seg20.c
	$(CC) $(CFLAGS) $< $(LDFLAGS) -S -fverbose-asm -o $@

clean:
	-rm $(TARGETS) $(obj)

.PHONY: test

test: $(TARGETS)
	flatpak run org.tuxfamily.hatari --harddrive . --frameskips 4 --mono
