#include "comp2.h"
#include <systemc>
#include <tlm>

namespace TTT {
	comp2::comp2(sc_core::sc_module_name name) :
		sc_core::sc_module(name),
		socks1("socks1"),
		socks2("socks2")
	{
		
		socks1.register_b_transport(this, &comp2::comp2_b_transport);
		//SC_THREAD(thread_process);
	}

	// for initaior socket 
	void comp2::thread_process()
	{

		// pass the payload like its a hot potato
		tlm::tlm_generic_payload* trans = trans_ptr;
		sc_core::sc_time delay = sc_core::sc_time(10, sc_core::SC_NS);
		tlm::tlm_response_status res = trans->get_response_status();
		socks2->b_transport(*trans, delay);
		wait(delay);
	}
	
	// for target socket
	void comp2::comp2_b_transport(
		tlm::tlm_generic_payload& trans, 
		sc_core::sc_time& delay) 
	{
		trans_ptr = &trans;
		cmd = trans.get_command();
		adr = trans.get_address()/4;
		ptr = trans.get_data_ptr();
		len = trans.get_data_length();
		byt = trans.get_byte_enable_ptr();
		wid = trans.get_streaming_width();
		

		std::cout << "RECEIVED BY COMP2 TARGET\n" 
			<< "TRANS = { \n"
			<< "COMMAND: " << cmd << ",\n"
			<< "ADDRESS: " << std::hex << adr << ",\n"
			<< "POINTER: " << std::hex << &ptr << ",\n"
			<< "  VALUE: " << std::hex << *reinterpret_cast<unsigned int*> (ptr) << ",\n"
			<< " LENGTH: " << len << ",\n"
			<< " SWIDTH: " << wid << "\n"
			<< "}\n" << std::endl;

		trans.set_response_status(tlm::TLM_OK_RESPONSE);
		trans_ptr = &trans;
		
		socks2->b_transport(trans, delay);
	}
}