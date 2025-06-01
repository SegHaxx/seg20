bin = i8080_tests
src = $(wildcard *.c)
obj = $(src:.c=.o)
CFLAGS = -g -Wall -pedantic -O3
LDFLAGS =

.PHONY: all clean

all: $(bin)

$(bin): $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)
	@size $@

clean:
	-rm $(bin) $(obj)
