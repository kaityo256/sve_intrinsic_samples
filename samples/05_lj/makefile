all: a.out
CC=aarch64-linux-gnu-g++
CPPFLAGS= -O3 -Wall -Wextra -static -march=armv8-a+sve -O2

a.out: lj.cpp
	$(CC) $(CPPFLAGS) $< -o $@

run:
	qemu-aarch64 ./a.out

.PHONY: clean

clean:
	rm -f a.out
