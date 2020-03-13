#include <systemc>
//#include "loader.hpp"
#include "vp.hpp"

using namespace sc_core;

int sc_main(int argc, char* argv[])
{
	vp uut("uut");

	sc_start(10, sc_core::SC_SEC);

    return 0;
}
