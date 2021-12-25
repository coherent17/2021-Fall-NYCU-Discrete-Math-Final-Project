CC = gcc
CFLAGS = -g -Wall
CHECKCC = valgrind
CHECKFLAGS = --leak-check=full -s

OBJS = Maximum_K_core

all: $(OBJS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

do:
	./Maximum_K_core < testcase1
	./Maximum_K_core < testcase2

check:
	$(CHECKCC) $(CHECKFLAGS) ./Maximum_K_core < testcase1
	$(CHECKCC) $(CHECKFLAGS) ./Maximum_K_core < testcase2

clean:
	rm -f $(OBJS)
