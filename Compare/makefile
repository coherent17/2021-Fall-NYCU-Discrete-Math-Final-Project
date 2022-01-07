CC = gcc
CFLAGS = -g -Wall -std=c99 -fopenmp

OBJS =  ParallelOptimize

all: $(OBJS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

do1:
	echo vertex = 1000, edge = 100
	time ./$(OBJS) 1000 1 < 1000V100E.in
	time ./$(OBJS) 1000 2 < 1000V100E.in
	time ./$(OBJS) 1000 3 < 1000V100E.in
	time ./$(OBJS) 1000 4 < 1000V100E.in


do2:
	echo vertex = 1000, edge = 1000
	time ./$(OBJS) 1000 1 < 1000V1000E.in
	time ./$(OBJS) 1000 2 < 1000V1000E.in
	time ./$(OBJS) 1000 3 < 1000V1000E.in
	time ./$(OBJS) 1000 4 < 1000V1000E.in


do3:
	echo vertex = 1000, edge = 10000
	time ./$(OBJS) 1000 1 < 1000V10000E.in
	time ./$(OBJS) 1000 2 < 1000V10000E.in
	time ./$(OBJS) 1000 3 < 1000V10000E.in
	time ./$(OBJS) 1000 4 < 1000V10000E.in


do4:
	echo vertex = 1000, edge = 100000
	time ./$(OBJS) 1000 1 < 1000V100000E.in
	time ./$(OBJS) 1000 2 < 1000V100000E.in
	time ./$(OBJS) 1000 3 < 1000V100000E.in
	time ./$(OBJS) 1000 4 < 1000V100000E.in


clean:
	rm -f $(OBJS)
