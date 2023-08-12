
.PHONY: all clean

CC=gcc

CFLAGS=-O2 -Wall -Wextra

SRCS=$(filter %.c, $(shell ls ./))
OBJS=$(SRCS: %.c = %.o)
OUTS=a.out

all: $(OUTS)

$(OUTS):
	$(CC) $(CFLAGS) $(SRCS) -o $@

clean:
	rm -rf *.o *.out
