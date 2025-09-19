TARGETS=i8080_tests seg20
src = $(wildcard *.c)
obj = $(src:.c=.o)
CFLAGS=-g -Wall -pedantic -Os
CFLAGS+=-Iportable
CFLAGS+=$(shell pkg-config --cflags tinfo)
LDFLAGS+=$(shell pkg-config --libs tinfo)

.PHONY: all clean

all: $(TARGETS)

i8080_tests: i8080_tests.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@size $@

seg20: seg20.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@size $@

clean:
	rm -f $(TARGETS) $(obj)
