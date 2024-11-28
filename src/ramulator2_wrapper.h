#include "debug/memview.h"
#include "memory/memory.h";

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif


EXTERNC void ramulator_init();
EXTERNC void ramulator_finish();

EXTERNC int  ramulator_send(Mem_Req* scarab_req);
EXTERNC void ramulator_tick();

EXTERNC int ramulator_get_chip_width();
EXTERNC int ramulator_get_chip_size();
EXTERNC int ramulator_get_num_chips();
EXTERNC int ramulator_get_chip_row_buffer_size();

EXTERNC Mem_Req* ramulator_search_queue(long phys_addr, Mem_Req_Type type);
EXTERNC int      try_completing_request(Mem_Req* req);
//{
//   //   std::cout << "Why is it not emptying re: " <<
//   //   mem->l1fill_queue.entry_count
//   //             << "\n";
//   if((unsigned int)mem->l1fill_queue.entry_count < 32) {
//     mem_complete_bus_in_access(
//       req, 0 /*mem->mem_queue.base[ii].priority*/);  // TODO_hasan: how do
//     // mem->mem_queue.base[ii].priority =
//     //   Mem_Req_Priority_Offset[MRT_MIN_PRIORITY];
//     // memview_memqueue(MEMVIEW_MEMQUEUE_DEPART, req);


//     return 1;
//   }

// return 0;
// }