CC   = gcc
CXX  = g++
DEFAULT_C_FLAGS   = -g -static
DEFAULT_CXX_FLAGS = -g -static

.PHONY: all test_qsort simple_array

all: test_qsort simple_array fib

test_qsort: fft.c ../utils/scarab_markers.h
	$(CC) -o fft fft.c $(DEFAULT_C_FLAGS) -O2 -lm

simple_array: simple_array.cc ../utils/scarab_markers.h
	$(CXX) -o simple_array simple_array.cc $(DEFAULT_CXX_FLAGS) -O0

fib: fib.c ../utils/scarab_markers.h
	$(CC) -o fib fib.c $(DEFAULT_C_FLAGS) -O3

clean:
	rm -f fft simple_array fib


