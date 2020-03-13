#include "memory.hpp"
#include "vp_addr.hpp"
#include <iterator>
#include <map>
#include <iostream>

using namespace std;
using namespace sc_core;
using namespace tlm;
using namespace sc_dt;

memory::memory(sc_module_name name):
  sc_module(name),
  ld_tsoc("ld_tsoc"),
  mem_tsoc("mem_tsoc")
{
	ld_tsoc.register_b_transport(this, &memory::b_transport);
	mem_tsoc.register_b_transport(this, &memory::b_transport);
 
}

vector<int> dram;
vector<int> encrypted;

void memory::b_transport(pl_t& pl, sc_time& offset)
{
	tlm_command    cmd  = pl.get_command();
	uint64         addr = pl.get_address();
	unsigned char* data = pl.get_data_ptr();
	unsigned int len = pl.get_data_length();

	vector<int> buff;
	vector<int> buff_enc;


		switch(cmd)
		{
		case TLM_WRITE_COMMAND:
		{
			switch(addr){		
				case(VP_ADDR_MEMORY_DATA):

					buff = *((vector<int>*)pl.get_data_ptr());
		
					for (int i = 0; i < len ; i++){

						dram.push_back(buff[i]);
					}
					break;
				case(VP_ADDR_MEMORY_END):

					cout<<"MEMORY: data received after ENCRYPTION "<<endl;
					encrypted = *((vector<int>*)pl.get_data_ptr());
		
					break;
			}
		pl.set_response_status( TLM_OK_RESPONSE );
		msg(pl);
		break;
		}	 
		case TLM_READ_COMMAND:
		{
			switch(addr){
				case(VP_ADDR_MEMORY_DATA):
																							   						cout<<"MEMORY: sending data to CORE "<<endl;
					pl.set_data_ptr((unsigned char*)&dram);
					pl.set_data_length(dram.size());
					pl.set_response_status( TLM_OK_RESPONSE );

				break;
				
				case(VP_ADDR_MEMORY_END):
																							   						cout<<"MEMORY: sending data to LOADER "<<endl;
					pl.set_data_ptr((unsigned char*)&encrypted);
					pl.set_data_length(encrypted.size());
					pl.set_response_status( TLM_OK_RESPONSE );

				break;
			}
			
			
			pl.set_response_status( TLM_OK_RESPONSE );
			msg(pl);
			break;
		}
		default:
			pl.set_response_status( TLM_COMMAND_ERROR_RESPONSE );
			SC_REPORT_ERROR("MEMORY", "TLM bad command");
		}

		
		
}

void memory::msg(const pl_t& pl)
{
	/*stringstream ss;
	ss << hex << pl.get_address();
	sc_uint<8> val = *((sc_uint<8>*)pl.get_data_ptr());
	string cmd  = pl.get_command() == TLM_READ_COMMAND ? "read  " : "write ";

	string msg = cmd + "val: " + to_string((int)val) + " adr: " + ss.str();
	msg += " @ " + sc_time_stamp().to_string();

	SC_REPORT_INFO("MEMORY", msg.c_str());*/
}
