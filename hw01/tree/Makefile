
.PHONY: all clean

CC=gcc
#CFLAGS=-O2 -Wall -Wextra
CFLAGS=-O2 -w

MAIN=test.c
EXE=test.out

SRCS=$(filter-out $(MAIN), $(wildcard *.c))
OBJS=$(SRCS: %.c = %.o)

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(MAIN) -o $@ $^

clean:
	rm -rf *.o *.out
