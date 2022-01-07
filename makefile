CC = gcc
CFLAGS = -g -Wall
CHECKCC = valgrind
CHECKFLAGS = --leak-check=full -s

OBJS = triangular_matrix

all: $(OBJS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

do:
	./$(OBJS) < testcase1
	./$(OBJS) < testcase2

check:
	$(CHECKCC) $(CHECKFLAGS) ./$(OBJS) < testcase1
	$(CHECKCC) $(CHECKFLAGS) ./$(OBJS) < testcase2

clean:
	rm -f $(OBJS)
