#include "vp.hpp"
#include <iostream>

using namespace sc_core;

vp::vp(sc_module_name name) :
	sc_module(name),
	ld("loader"),
	ic("interconnect"),
	co("core"),
	mem("memory")
{
	ld.mem_isoc.bind(mem.ld_tsoc);
	ld.int_isoc.bind(ic.c_cpu);
	ic.s_core.bind(co.core_tsoc);
	co.core_isoc.bind(mem.mem_tsoc);
//	sb.s_timer.bind(t.soc);
	SC_REPORT_INFO("VP", "Platform is constructed");
}
