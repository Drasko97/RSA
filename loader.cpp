#include "loader.hpp"
#include "vp_addr.hpp"
#include <string>
#include <fstream>
#include <tlm_utils/tlm_quantumkeeper.h>
#include <iostream>
#include <iterator>
#include <vector>


using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm;

ifstream tekst("tekst.txt");

SC_HAS_PROCESS(loader);
loader::loader(sc_module_name name):
	sc_module(name)
{
	SC_THREAD(test);
}

int gcd(int a, int b){
	if(b==0)
	return a;
	return gcd(b, a%b);
}

void loader::prime(int n) {		
	int i=0, j=0;

	for(i=1;i<=n;i++) {
		if((n%i)==0){
			j++;
		}
	}

	if(j==2){
		prime_num=1;
	}else {
		prime_num=0;
	}
}

void loader::test()
{	

//////
sc_time loct;
tlm_generic_payload pl;
tlm_utils::tlm_quantumkeeper qk;
qk.reset();
//////

//////CALCULATING THE VALUES OF KEYS-BEG
	int i=0, j=0;

	while(i == 0) {
		std::cout<<"Enter prime number p:" << endl;
		std::cin>> p;
		prime(p);

		if(prime_num==0){
			std::cout<<"Number is not prime."<< endl;
			prime_num=0;
			i=0;
		}
		else{
			i=1;
		}
		
	}

	while(j == 0){
		std::cout<<"Enter prime number q:" << endl;
		std::cin>> q;		
		prime(q);

		if(prime_num==0){
			std::cout<<"Number is not prime." << endl;
			prime_num=0;
			j=0;
		}
		else {
			j=1;
		}	

	}

	int r, Phi, x, k;
	int Pk, e, d;
	Pk=p*q;
	Phi=(p-1)*(q-1);

	x=2;
	e=1;

	while (x>1){
		e=e+1;
		x=gcd(Phi, e);
	}

	i=1;
	r=1;

	while(r>0){
		k=(Phi*i)+1;
		r=k%e;
		i=i+1;
	}

	d=k/e;

	cout<< "Value N: " << Pk <<endl;
	cout<< "Value of public key-a e: "<< e << endl;
	cout<< "Value Phi: "<<  Phi <<endl;
	cout<< "Value of private key-a d: "<< d << endl;

//////CALCULATING THE VALUES OF KEYS-END

//////SENDING KEYS TO CORE - BEG
	sc_uint<64> val[3];
	val[0]=e;
	val[1]=Phi;
	val[2]=d;
	for (i = 0; i !=3; ++i)
	{
		switch(i)
		{
		case 0:
			pl.set_address(VP_ADDR_CORE_E);
			break;
		case 1:
			pl.set_address(VP_ADDR_CORE_PUBLIC);
			break;
		case 2: 
			pl.set_address(VP_ADDR_CORE_PRIVATE);
			break;
		default:
			pl.set_address(VP_ADDR_CORE);
			break;
		}
		pl.set_command(TLM_WRITE_COMMAND);
		pl.set_data_length(1);
		pl.set_data_ptr((unsigned char*)&val[i]);
		pl.set_response_status (TLM_INCOMPLETE_RESPONSE);
		int_isoc->b_transport(pl, loct);
		qk.set_and_sync(loct);
		msg(pl);

		loct += sc_time(5, SC_NS);
	}
//////SENDING KEYS TO CORE - END

//////LOADING THE FILE - BEG
	int length=0;
	int input[20];

	char text;
	fstream inFile("tekst.txt");
	inFile>>std::noskipws;

	if(!inFile)
	{
		std::cout<<"Error opening the file. \n";
	}
	
	while(inFile >> text){
		input[length]=text;
		length++;		
	}	 


	inFile.close();
	std::cout<<endl;
	
//////LOADING THE FILE-END


//////SENDING DATA LENGTH IN CORE - BEG
	sc_uint<64> len=length;
	cout<<"SEND DATA LENGTH TO CORE:"<<len<<endl;
	pl.set_command (TLM_WRITE_COMMAND);
	pl.set_address (VP_ADDR_CORE_LENGTH);
	pl.set_data_ptr ((unsigned char*)&len);
	pl.set_data_length(1);
	pl.set_response_status(TLM_INCOMPLETE_RESPONSE);

	int_isoc->b_transport(pl, loct);
	qk.set_and_sync(loct);
	msg(pl);

	loct +=sc_time(5, SC_NS);
//////SENDING DATA LENGTH IN CORE - END

//////SENDING DATA TO MEMORY - BEG

	int f=0;
	int last_part=0;	
	f=length/8;	
	vector<int> array1 (length);
	
	

	for(int i =0; i<=f; i++){
		array1.clear();
		
		
		for(int j=0; j<8; j++){
			array1.push_back(input[i*8+j]);
		}
			
		if(i==f){
			last_part=((length)%8);
			for(int j=0; j<last_part;j++){
			}
		}
			
		pl.set_address(VP_ADDR_MEMORY_DATA);
		pl.set_command(TLM_WRITE_COMMAND);
		pl.set_data_length(8);
		pl.set_data_ptr((unsigned char*)&array1);
		pl.set_response_status (TLM_INCOMPLETE_RESPONSE);


		mem_isoc->b_transport(pl, loct);
		qk.set_and_sync(loct);
		msg(pl);
		loct += sc_time(5, SC_NS);
		
	}

	pl.set_address(VP_ADDR_CORE_START_MEM);
	pl.set_command(TLM_READ_COMMAND);	
	pl.set_response_status (TLM_INCOMPLETE_RESPONSE);
	
	int_isoc->b_transport(pl, loct);
	qk.set_and_sync(loct);
	loct += sc_time(5, SC_NS);

//////SENDING DATA TO MEMORY - END

//////RECIEVING ENCRYPTED DATA - BEG
	vector<int> encrypted_loader(length);

	pl.set_address(VP_ADDR_MEMORY_END);
	pl.set_command(TLM_READ_COMMAND);
	pl.set_response_status (TLM_INCOMPLETE_RESPONSE);

	mem_isoc->b_transport(pl, loct);
	qk.set_and_sync(loct);
	loct += sc_time(5, SC_NS);

	encrypted_loader = *((vector<int>*)pl.get_data_ptr());
	cout<<"LOADER: data received from MEMORY after encryption "<<endl;

//////RECIEVING ENCRYPTED DATA - END

//////WRITING IN FILE - BEG
	ofstream outfile;
	ofstream checkfile;
    	checkfile.open("encrypted.txt");

	if(checkfile.is_open()){
		for(int i=1; i<length; i++){
			checkfile<<encrypted_loader[i]<<" ";
		}
	}
//////WRITING IN FILE - END

}


void loader::msg(const pl_t& pl)
{
	/*static int trcnt = 0;
	stringstream ss;
	ss << hex << pl.get_address();
	sc_uint<32> val = *((sc_uint<32>*)pl.get_data_ptr());

	string msg = "val: " + to_string((int)val) + " adr: " + ss.str();
	msg += " @ " + sc_time_stamp().to_string();

	SC_REPORT_INFO("TB", msg.c_str());
	trcnt++;
	SC_REPORT_INFO("TB", ("------------" + to_string(trcnt)).c_str());*/
}
