#include "core.hpp"
#include "vp_addr.hpp"
#include <tlm_utils/tlm_quantumkeeper.h>
#include <cmath>

using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm;

SC_HAS_PROCESS(core);
core::core(sc_module_name name):
	sc_module(name),
	core_tsoc("soc"),
	core_isoc("isoc")
{
	core_tsoc.register_b_transport(this, &core::b_transport);
}

sc_time loct;
tlm_generic_payload pl;
tlm_utils::tlm_quantumkeeper qk;

void core::b_transport(pl_t& pl, sc_time& offset)
{
	tlm_command    cmd  = pl.get_command();
	uint64         addr = pl.get_address();
	unsigned char *data = pl.get_data_ptr();

	vector<int> array;	
	
	

		switch(cmd)
		{
		case TLM_WRITE_COMMAND:
		{
			switch(addr){
				/*case VP_ADDR_CORE:
					val = *((sc_uint<8>*)pl.get_data_ptr());
					pl.set_response_status( TLM_OK_RESPONSE );
					msg(pl);	
					break;*/				
				/*case CORE_START:
					ctrl =*((sc_uint<4>*)data);
					ctrl1 = ctrl;
					//cout<<"ctrl_Gore:"<<ctrl<<endl;	
					pl.set_response_status(TLM_OK_RESPONSE);
					break;*/
				case CORE_E:					
					e = *((sc_uint<8>*)pl.get_data_ptr());
					key_e=e;
					//cout<<"KEY_e gore:"<<key_e<<endl;			
					pl.set_response_status( TLM_OK_RESPONSE );
					msg(pl);	
					break;
				case CORE_PRIVATE:
					priv = *((sc_uint<8>*)pl.get_data_ptr());
					key_priv = priv;
					pl.set_response_status( TLM_OK_RESPONSE );
					msg(pl);	
					break;
				case CORE_PUBLIC:
					pub = *((sc_uint<8>*)pl.get_data_ptr());
					key_pub = pub;
					//cout<<"KEY_PUB gore:"<<key_pub<<endl;
					pl.set_response_status( TLM_OK_RESPONSE );
					msg(pl);	
					break;
				case CORE_LENGTH:
					length = *((sc_uint<8>*)pl.get_data_ptr());
					length1 = length;
					pl.set_response_status( TLM_OK_RESPONSE );
					msg(pl);	
					break;
				default:
					pl.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
					break;
			}
			pl.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
			break;
			
		}

		case TLM_READ_COMMAND:
		{
			switch(addr){
				case CORE_START_MEM:
									
					pl.set_address(VP_ADDR_MEMORY_DATA);
					pl.set_command(TLM_READ_COMMAND);
					pl.set_response_status (TLM_INCOMPLETE_RESPONSE);

					core_isoc->b_transport(pl, loct);
					qk.set_and_sync(loct);
					loct += sc_time(5, SC_NS);

					array = *((vector<int>*)pl.get_data_ptr());
					int i;

					for(i = 0; i < length1; i++){
						niz[i]=array[i];
						//cout<<"niz:"<<niz[i]<<endl;

						/*if(i==length1-1){
						cout<<"USLOOOO"<<endl;
						encrypt();	
						}*/
					}
					
					
					encrypt();
					cout<<"CORE: data received from MEMORY "<<endl;
					pl.set_response_status(TLM_OK_RESPONSE);
				break;
			}	
		
			
			pl.set_response_status( TLM_OK_RESPONSE );
			msg(pl);
			break;
		}
		default:
			pl.set_response_status( TLM_COMMAND_ERROR_RESPONSE );
			SC_REPORT_ERROR("CORE", "TLM bad command");
		}

		
}


void core::encrypt(){

	
	vector<int> cipher(length1);	

	for(int j=1; j<length1; j++){
		cipher[j]=crypt(niz[j], key_pub, key_e);
		//std::cout<<cipher[j]<< " ";	
	}
		

	pl.set_address(VP_ADDR_MEMORY_END);
	pl.set_command(TLM_WRITE_COMMAND);	
	pl.set_data_length(cipher.size());
	pl.set_data_ptr((unsigned char*)&cipher);
	pl.set_response_status (TLM_INCOMPLETE_RESPONSE);
	
	core_isoc->b_transport(pl, loct);
	qk.set_and_sync(loct);
	loct += sc_time(5, SC_NS);
	

	

	
	
}

int core::BitSet(int n, int k){
		if((n)&(1<<(k))){
			return 1;
		}else {
			return 0;
		}	
	}

int core::crypt(int test2, int N, int e){
	
		long cf;
		//int cf;
		long test1;
		//int test1;
		int j, i;
		int k=21;
		cf=1;


		test1=test2;
		//if(N!=0){

		cf=(test1*cf)%N;
		
		for(i=k-1;i>0;i--){
			test1=(test1*test1)%N;
			j=k-i;
			if(BitSet(e, j)){
				cf=((test1*cf)%N);
			}

		}
	
		return cf;
		//}
	

}

void core::msg(const pl_t& pl)
{
/*	stringstream ss;
	ss << hex << pl.get_address();
	sc_uint<8> val = *((sc_uint<8>*)pl.get_data_ptr());
	string cmd  = pl.get_command() == TLM_READ_COMMAND ? "read  " : "write ";

	string msg = cmd + "val: " + to_string((int)val) + " adr: " + ss.str();
	msg += " @ " + sc_time_stamp().to_string();

	SC_REPORT_INFO("CORE", msg.c_str());*/
}
