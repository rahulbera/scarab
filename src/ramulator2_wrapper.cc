#include "ramulator2_wrapper.h"
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include "ramulator.param.h"
#include "ramulator2/src/base/base.h"
#include "ramulator2/src/base/request.h"

#include "ramulator2/src/base/config.h"

#include "ramulator2/src/frontend/frontend.h"


#include "ramulator2/src/memory_system/memory_system.h"
std::string                           config_path;
Ramulator::IFrontEnd*                 ramulator2_frontend;
Ramulator::IMemorySystem*             ramulator2_memorysystem;
std::map<long, std::list<Mem_Req*>>   inflight_read_reqs;
std::deque<std::pair<long, Mem_Req*>> resp_queue;  // completed read request
                                                   // that need
YAML::Node config;
long       number_of_sends     = 0;
long       number_of_callbacks = 0;
// std::map<long, Mem_Req*> inflight_read_reqs;
void ramulator_callback(Ramulator::Request req) {
  // std::cout << "Ramulator Callback()" << req.addr << "\n";
  number_of_callbacks++;
  auto it_scarab_req = inflight_read_reqs.find(req.addr);
  for(auto req : it_scarab_req->second)
    resp_queue.push_back(std::make_pair(it_scarab_req->first, req));
  // resp_queue.push_back(make_pair(it_scarab_req->first,
  // it_scarab_req->second));
  inflight_read_reqs.erase(it_scarab_req);
}
void to_ramulator_req(const Mem_Req*      scarab_req,
                      Ramulator::Request* ramulator_req) {
  if(scarab_req->type == MRT_WB)
    ramulator_req->type_id = Ramulator::Request::Type::Write;
  else if(scarab_req->type == MRT_DFETCH || scarab_req->type == MRT_DSTORE ||
          scarab_req->type == MRT_IFETCH || scarab_req->type == MRT_IPRF ||
          scarab_req->type == MRT_DPRF)
    ramulator_req->type_id = Ramulator::Request::Type::Read;

  ramulator_req->addr      = scarab_req->phys_addr;
  ramulator_req->source_id = scarab_req->proc_id;

  ramulator_req->callback = ramulator_callback;
}
Mem_Req ramulator_to_scarab_request(Ramulator::Request req) {}
void    ramulator_init() {
  config_path = RAMULATOR_CONFIG_PATH;
  // std::cout << "Ramulator init()\n";
  config = Ramulator::Config::parse_config_file(config_path, {});


  ramulator2_frontend = Ramulator::Factory::create_frontend(config);


  ramulator2_memorysystem = Ramulator::Factory::create_memory_system(config);


  ramulator2_frontend->connect_memory_system(ramulator2_memorysystem);


  ramulator2_memorysystem->connect_frontend(ramulator2_frontend);
}
int try_completing_request(Mem_Req* req) {
  // std::cout << "Why is it not emptying re: " << mem->l1fill_queue.entry_count
  //           << "\n";
  if((unsigned int)mem->l1fill_queue.entry_count < 32) {
    mem_complete_bus_in_access(
      req, 0 /*mem->mem_queue.base[ii].priority*/);  // TODO_hasan: how do
    // mem->mem_queue.base[ii].priority=Mem_Req_Priority_Offset[MRT_MIN_PRIORITY];
    // mem_queue_removal_count++;
    // l1fill_queue_insertion_count++;
    // mem->mem_queue.base[ii].priority =
    //   Mem_Req_Priority_Offset[MRT_MIN_PRIORITY];
    memview_memqueue(MEMVIEW_MEMQUEUE_DEPART, req);


    return 1;
  }

  return 0;
}

void ramulator_finish() {
  // std::cout << "Ramulator finish()\n";
  // std::cout << "Number of sends : " << number_of_sends << "\n";

  std::streambuf*    oldCoutStreamBuf = std::cout.rdbuf();
  std::ostringstream strCout;
  std::cout.rdbuf(strCout.rdbuf());

  ramulator2_frontend->finalize();
  ramulator2_memorysystem->finalize();

  std::cout.rdbuf(oldCoutStreamBuf);
  std::ofstream myfile;
  myfile.open("./ramulator.out");
  myfile << strCout.str();
  myfile.close();
}

int ramulator_send(Mem_Req* scarab_req) {
  // std::cout << "Ramulator send with addr : " << scarab_req->phys_addr << " "
  //  << scarab_req->addr << "\n";
  number_of_sends++;
  bool                enqueue_success = false;
  auto                it_req  = inflight_read_reqs.find(scarab_req->phys_addr);
  Ramulator::Request* req_ram = new Ramulator::Request(
    scarab_req->addr, Ramulator::Request::Type::Read);

  to_ramulator_req(scarab_req, req_ram);
  //   if(scarab_req->phys_addr == 169025721946813184)
  //     std::cout << "It happened reeee\n";
  if(it_req != inflight_read_reqs.end() &&
     (scarab_req->type == MRT_DFETCH || scarab_req->type == MRT_DSTORE ||
      scarab_req->type == MRT_IFETCH || scarab_req->type == MRT_IPRF ||
      scarab_req->type == MRT_DPRF)) {
    it_req->second.push_back(scarab_req);
    scarab_req->mem_queue_cycle = cycle_count;
    // std::cout << "We are here\n";
    return 1;
  }

  enqueue_success = ramulator2_frontend->receive_external_requests(
    req_ram->type_id, scarab_req->phys_addr, scarab_req->proc_id,
    ramulator_callback);

  if(enqueue_success) {
    if(scarab_req->type == MRT_DFETCH || scarab_req->type == MRT_DSTORE ||
       scarab_req->type == MRT_IFETCH || scarab_req->type == MRT_IPRF ||
       scarab_req->type == MRT_DPRF) {
      if(it_req != inflight_read_reqs.end()) {
        it_req->second.push_back(scarab_req);
      } else {
        std::list<Mem_Req*> temp_list;
        temp_list.push_back(scarab_req);
        inflight_read_reqs.insert({scarab_req->phys_addr, temp_list});
      }
    }
    scarab_req->mem_queue_cycle = cycle_count;
  }
  // if(scarab_req->type == Mem_Req_Type::MRT_DFETCH) {


  return enqueue_success;
  //}
  // return 1;
}
void ramulator_tick() {
  // std::cout << "Ramulator tick()\n";
  ramulator2_frontend->tick();
  ramulator2_memorysystem->tick();
  if(resp_queue.size() > 0) {
    if(try_completing_request(resp_queue.front().second))
      // std::cout << "Good ram tick\n";
      resp_queue.pop_front();
  }
}

int ramulator_get_chip_width() {
  std::cout << "Ramulator width()\n";
}
int ramulator_get_chip_size() {
  std::cout << "Ramulator chip size()\n";
}
int ramulator_get_num_chips() {
  std::cout << "Ramulator num ships()\n";
}
int ramulator_get_chip_row_buffer_size() {
  std::cout << "Ramulator Row Buffer size()\n";
}

Mem_Req* ramulator_search_queue(long phys_addr, Mem_Req_Type type) {
  //   if(phys_addr == 169025721946813184)
  //     std::cout << "Ramulator Search queue() with addr : " << phys_addr <<
  //     "\n";
  auto it_req = inflight_read_reqs.find(phys_addr);

  // Search request queue
  if(it_req != inflight_read_reqs.end()) {
    for(auto req : it_req->second) {
      if((req->type == MRT_IFETCH || req->type == MRT_IPRF) &&
         (type == MRT_IFETCH || type == MRT_IPRF)) {
        // std::cout << "Returned here " << __LINE__ << "\n";
        return req;
      } else if((req->type == MRT_DFETCH || req->type == MRT_DPRF ||
                 req->type == MRT_DSTORE) &&
                (type == MRT_DFETCH || type == MRT_DPRF ||
                 type == MRT_DSTORE)) {
        // std::cout << "Returned here " << __LINE__ << "\n";
        return req;
      }
    }
  }

  // Search response queue
  for(auto resp : resp_queue) {
    if(resp.first == phys_addr) {
      if((resp.second->type == MRT_IFETCH || resp.second->type == MRT_IPRF) &&
         (type == MRT_IFETCH || type == MRT_IPRF)) {
        // std::cout << "Returned here " << __LINE__ << "\n";
        return resp.second;
      }

      else if((resp.second->type == MRT_DFETCH ||
               resp.second->type == MRT_DPRF ||
               resp.second->type == MRT_DSTORE) &&
              (type == MRT_DFETCH || type == MRT_DPRF || type == MRT_DSTORE)) {
        // std::cout << "Returned here " << __LINE__ << "\n";
        return resp.second;
      }
    }
  }
  //   if(phys_addr == 169025721946813184)
  //     std::cout << "Returned here " << __LINE__ << " "
  //               << inflight_read_reqs.size() << " " << resp_queue.size() << "
  //               "
  //               << number_of_sends << " " << number_of_callbacks << " "
  //               << cycle_count << "\n";
  return NULL;
}
