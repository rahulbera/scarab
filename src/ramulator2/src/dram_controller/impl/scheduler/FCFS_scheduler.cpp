#include <vector>

#include "base/base.h"
#include "dram_controller/controller.h"
#include "dram_controller/scheduler.h"

namespace Ramulator {

class FCFS : public IScheduler, public Implementation {
  RAMULATOR_REGISTER_IMPLEMENTATION(IScheduler, FCFS, "FCFS",
                                    "FCFS DRAM Scheduler.")
 private:
  IDRAM* m_dram;

 public:
  void init() override {};
  int  total_number_of_bits;
  void setup(IFrontEnd* frontend, IMemorySystem* memory_system) override {
    m_dram               = cast_parent<IDRAMController>()->m_dram;
    total_number_of_bits = 0;
  };

  ReqBuffer::iterator compare(ReqBuffer::iterator req1,
                              ReqBuffer::iterator req2) override {
    bool ready1 = m_dram->check_ready(req1->command, req1->addr_vec);
    bool ready2 = m_dram->check_ready(req2->command, req2->addr_vec);


    // Fallback to FCFS
    if(req1->arrive <= req2->arrive) {
      return req1;
    } else {
      return req2;
    }
  }

  ReqBuffer::iterator get_best_request(ReqBuffer& buffer) override {
    if(buffer.size() == 0) {
      return buffer.end();
    }
    // std::cout << "Buffer has size of " << buffer.size() << "\n";
    if(buffer.size() > 1) {
      total_number_of_bits += std::log2((double)buffer.size());
    }
    for(auto& req : buffer) {
      req.command = m_dram->get_preq_command(req.final_command, req.addr_vec);
      // std::cout << req.addr << " ";
    }
    // std::cout << "\n";
    auto candidate = buffer.begin();
    for(auto next = std::next(buffer.begin(), 1); next != buffer.end();
        next++) {
      candidate = compare(candidate, next);
    }
    return candidate;
  }
};

}  // namespace Ramulator
