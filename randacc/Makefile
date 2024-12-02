CXX=/usr/bin/g++
CXX_FLAGS= -O3


EXTRA_CXX_FLAGS = -DSNIPER_SIM
SNIPER_ROOT = /mnt/batty/kanellok/virtuoso_private/
CXX_FLAGS += -I$(SNIPER_ROOT)/include -L$(SNIPER_ROOT)/lib -I../


CXX_FLAGS += $(EXTRA_CXX_FLAGS)

all: randacc_walk_counter randacc_walk_latency

randacc_walk_counter: randacc.c
	${CXX} ${CXX_FLAGS} randacc.c ../perf_event_walk_counter.o -o randacc_walk_counter

randacc_walk_latency: randacc.c
	${CXX} ${CXX_FLAGS} randacc.c ../perf_event_walk_latency.o -o randacc_walk_latency

clean:
	rm -rf *.o
	rm -rf randacc_walk_counter
	rm -rf randacc_walk_latency

.PHONY: all clean 
