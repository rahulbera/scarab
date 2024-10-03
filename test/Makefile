CC   = gcc
CXX  = g++ 

.PHONY: all test_qsort simple_array

all: test_qsort simple_array

test_qsort: fft.c ../utils/scarab_markers.h
	$(CC) -o fft fft.c -O2 -lm -static

simple_array: simple_array.cc ../utils/scarab_markers.h
	$(CXX) -o simple_array simple_array.cc -O0 -g -static

clean:
	rm -f fft simple_array


