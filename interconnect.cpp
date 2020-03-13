#include "interconnect.hpp"
#include "vp_addr.hpp"
#include <string>

using namespace std;
using namespace tlm;
using namespace sc_core;
using namespace sc_dt;

interconnect::interconnect(sc_module_name name) :
	sc_module(name)
{
	c_cpu.register_b_transport(this, &interconnect::b_transport);
}

void interconnect::b_transport(pl_t& pl, sc_core::sc_time& offset)
{
	uint64 addr = pl.get_address();
	uint64 taddr;
	offset += sc_time(2, SC_NS);
	
	if(addr >= VP_ADDR_CORE && addr <= VP_ADDR_CORE_START_MEM){
		taddr= addr & 0x0000000F;
		pl.set_address(taddr);
		s_core->b_transport(pl, offset);
		msg(pl);
	}

	pl.set_address(addr);
}

void interconnect::msg(const pl_t& pl)
{
/*	stringstream ss;
	ss << hex << pl.get_address();
	sc_uint<32> val = *((sc_uint<32>*)pl.get_data_ptr());
	string cmd  = pl.get_command() == TLM_READ_COMMAND ? "read  " : "write ";

	string msg = cmd + "val: " + to_string((int)val) + " adr: " + ss.str();
	msg += " @ " + sc_time_stamp().to_string();

	SC_REPORT_INFO("BUS FWD", msg.c_str());*/
}
