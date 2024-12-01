#include <stdint.h>
#include <stdio.h>
#include "../utils/scarab_markers.h"

#define BASE 15
#define MAX_ITER 20

uint64_t fib(uint32_t n) {
  if(n == 0 || n == 1)
    return n;
  else
    return fib(n - 1) + fib(n - 2);
}

int main(int argc, char* argv[]) {
  uint64_t fib_32 = fib(32);

  for(uint32_t i = BASE; i < BASE + MAX_ITER; ++i) {
    printf("Fib(%u) = %lu\n", i, fib(i));
  }

  scarab_begin();
  printf("Fib(%u) = %lu\n", (BASE + MAX_ITER), fib(BASE + MAX_ITER));
  scarab_end();

  return 0;
}