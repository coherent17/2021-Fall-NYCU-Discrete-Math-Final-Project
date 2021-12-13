CC = gcc
CFLAGS = -g -Wall

OBJS = Maximum_K_core

all: $(OBJS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS)
