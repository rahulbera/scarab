#include "../utils/scarab_markers.h"

#define NUM_ELEMS 100000
#define NUM_PARTITIONS 16
#define PART_DELTA 0x0505

int main(int argc, char* argv[]) {
  uint64_t inp_arr[NUM_ELEMS], out_arr[NUM_ELEMS];
  uint64_t curr_part_val = 0;
  for(uint32_t index = 0; index < NUM_ELEMS; ++index) {
    if(index && (index % NUM_PARTITIONS != 0)) {
      inp_arr[index] = curr_part_val;
    } else {
      if(index)
        curr_part_val += PART_DELTA;
      inp_arr[index] = curr_part_val;
    }
  }

  // Region of interest
  scarab_begin();
  for(uint32_t index = 0; index < NUM_ELEMS; ++index) {
    out_arr[index] = inp_arr[index] << 2;
  }
  scarab_end();

  return 0;
}