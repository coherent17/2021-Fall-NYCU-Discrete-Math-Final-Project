CC = gcc
CFLAGS = -g -Wall

OBJS = Maximum_K_core

all: $(OBJS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

do:
	./Maximum_K_core < testcase1
	./Maximum_K_core < testcase2

clean:
	rm -f $(OBJS)
