
.PHONY: all clean

CC=gcc
#CFLAGS=-O2 -Wall -Wextra
CFLAGS=-O2 -w

ST_MAIN=sttest.c
RB_MAIN=rbtest.c
ST_EXE=sttest.out
RB_EXE=rbtest.out

SRCS=$(filter-out $(ST_MAIN) $(RB_MAIN), $(wildcard *.c))
OBJS=$(SRCS: %.c = %.o)

all: $(ST_EXE) $(RB_EXE)

$(ST_EXE): $(OBJS)
	$(CC) $(CFLAGS) $(ST_MAIN) -o $@ $^

$(RB_EXE): $(OBJS)
	$(CC) $(CFLAGS) $(RB_MAIN) -o $@ $^

clean:
	rm -rf *.o *.out
