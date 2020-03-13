#ifndef _VP_HPP_
#define _VP_HPP_

#include <systemc>
#include "interconnect.hpp"
#include "core.hpp"
#include "loader.hpp"
#include "memory.hpp"

class vp :
	sc_core::sc_module
{
public:
	vp(sc_core::sc_module_name);

protected:
	interconnect ic;
	core co;
	loader ld;
	memory mem;

};

#endif
