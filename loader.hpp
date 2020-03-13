#ifndef _LOADER_H_
#define _LOADER_H_

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>

class loader :
	public sc_core::sc_module
{
public:
	loader(sc_core::sc_module_name);

	tlm_utils::simple_initiator_socket<loader> int_isoc;
	tlm_utils::simple_initiator_socket<loader> mem_isoc;
protected:
	int p, q;
	int prime_num=0;

	void test();
	
	void prime(int n);
	typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;

	void msg(const pl_t&);
};


#endif
