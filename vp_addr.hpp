#ifndef _VP_TSP_ADDR_H_
#define _VP_TSP_ADDR_H_

#include "core.hpp"

const sc_dt::uint64 VP_ADDR_CORE = 0x43C00000;
const sc_dt::uint64 VP_ADDR_CORE_START= VP_ADDR_CORE + CORE_START;
const sc_dt::uint64 VP_ADDR_CORE_E= VP_ADDR_CORE + CORE_E;
const sc_dt::uint64 VP_ADDR_CORE_PRIVATE= VP_ADDR_CORE + CORE_PRIVATE;
const sc_dt::uint64 VP_ADDR_CORE_PUBLIC= VP_ADDR_CORE + CORE_PUBLIC;
const sc_dt::uint64 VP_ADDR_CORE_LENGTH= VP_ADDR_CORE + CORE_LENGTH;
const sc_dt::uint64 VP_ADDR_CORE_START_MEM= VP_ADDR_CORE + CORE_START_MEM;
const sc_dt::uint64 VP_ADDR_MEMORY = 0x43B00000;
const sc_dt::uint64 VP_ADDR_MEMORY_DATA = 0x43B00001;
const sc_dt::uint64 VP_ADDR_MEMORY_END = 0x43B00002;
//const sc_dt::uint64 VP_ADDR_MEMORY_DATA = 10;
//const sc_dt::uint64 VP_ADDR_TIMER_CFG = VP_ADDR_TIMER + TIMER_CFG;
//const sc_dt::uint64 VP_ADDR_TIMER_CNT = VP_ADDR_TIMER + TIMER_CNT;
//const sc_dt::uint64 VP_ADDR_TIMER_RLD = VP_ADDR_TIMER + TIMER_RLD;
//const sc_dt::uint64 VP_ADDR_TIMER_H = 0x43D00004;

#endif
