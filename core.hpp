#ifndef _CORE_H_
#define _CORE_H_

#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <string.h>
#define SIZE 512

const sc_dt::uint64 CORE_START = 1;
const sc_dt::uint64 CORE_E = 2;
const sc_dt::uint64 CORE_PRIVATE = 3;
const sc_dt::uint64 CORE_PUBLIC = 4;
const sc_dt::uint64 CORE_LENGTH = 5;
const sc_dt::uint64 CORE_START_MEM = 6;

class core :
	public sc_core::sc_module
{
public:
	core(sc_core::sc_module_name);

	tlm_utils::simple_target_socket<core> core_tsoc;
	tlm_utils::simple_initiator_socket<core> core_isoc;
	
	sc_dt::sc_uint<64> ctrl;
	sc_dt::sc_uint<64> e;
	sc_dt::sc_uint<64> priv;
	sc_dt::sc_uint<64> pub;
	sc_dt::sc_uint<64> length;


protected:

	unsigned int key_e, key_priv, key_pub, length1, ctrl1;
	int niz[SIZE];
	//int cipher[24];

	void encrypt();

	int crypt(int test2, int N, int e);
	int BitSet(int n, int k);

	typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
	void b_transport(pl_t&, sc_core::sc_time&);
	void msg(const pl_t&);
};

#endif
