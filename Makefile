TARGETS=i8080_tests seg20
src = $(wildcard *.c)
obj = $(src:.c=.o)
CFLAGS=-g -Wall -pedantic -Ofast
CFLAGS+=-Iportable
LDFLAGS=

.PHONY: all clean

all: $(TARGETS)

i8080_tests: i8080_tests.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@size $@

seg20: seg20.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@size $@

clean:
	-rm $(TARGETS) $(obj)
