CC = gcc
CFLAGS = -g -Wall

OBJS = BruteForce BruteForceOptimize

all: $(OBJS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

do1:
	time ./BruteForce < testcase1
	time ./BruteForce < testcase2

do2:
	time ./BruteForceOptimize < testcase1
	time ./BruteForceOptimize < testcase2

clean:
	rm -f $(OBJS)
