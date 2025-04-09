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
		
		for (int i = 0; i < SIZE; i++) 
		{
			mem[i] = 0xDEAD|0xBEEF; 
		}
		SC_THREAD(thread_process);
	}

	// for initaior socket 
	void comp2::thread_process()
	{

		// pass the payload like its a hot potato
		tlm::tlm_generic_payload* trans = trans_ptr;
		sc_core::sc_time delay = sc_core::sc_time(10, sc_core::SC_NS);
		cmd = trans->get_command();
		adr = trans->get_address(); // address size on target.h
		ptr = trans->get_data_ptr();
		
		len = trans->get_data_length();
		wid = trans->get_streaming_width(); // = data_length to indicate no streaming
		byt = trans->get_byte_enable_ptr(); // 0 indicates unused
		
		std::cout << "[ini comp2] ptr " << ptr << ", " << 
		" trans->get_data_ptr " << std::hex << std::endl;


		std::cout << "trans = { " << cmd << ", " << std::hex << adr
			<< " } , data = " << std::hex << reinterpret_cast<sc_dt::uint64 *>(*ptr) << " at time " << sc_core::sc_time_stamp()
			<< " delay = " << delay << "\n" << std::endl;
		tlm::tlm_response_status res = trans->get_response_status();

		if (res == tlm::TLM_OK_RESPONSE)
			std::cout << "OK RES" << std::endl;
		else 
			std::cout << "NOT VERY OK RES" << std::endl;

	}

	// for target socket
	void comp2::comp2_b_transport(
		tlm::tlm_generic_payload& trans, 
		sc_core::sc_time& delay) 
	{
		tlm::tlm_command	cmd = trans.get_command();
		sc_dt::uint64		adr = trans.get_address() /4;
		unsigned char*		ptr = trans.get_data_ptr();
		unsigned int		len = trans.get_data_length();
		unsigned char*		byt = trans.get_byte_enable_ptr();
		unsigned int		wid = trans.get_streaming_width();

		std::cout << "[] trans.get_data_ptr() "<< reinterpret_cast<sc_dt::uint64*> (*ptr) << std::endl;

		if (adr >= sc_dt::uint64(SIZE) || byt != nullptr || len > 4 || wid < len)
		{
			SC_REPORT_ERROR("TLM-2", "comp3 does not support given generic payload transaction");
		};

		if (cmd == tlm::TLM_READ_COMMAND)
		{
			std::memcpy(ptr, &mem[adr], len);
		}
		else if (cmd == tlm::TLM_WRITE_COMMAND)
		{
			std::memcpy(&mem[adr], ptr, len);

		}
		else 
		{
			std::cout << "TLM-2 tlm_generic_payload null" << std::endl;
		};
		trans.set_response_status(tlm::TLM_OK_RESPONSE);
		
		trans_ptr = &trans;
		
		// NEW IDEA CREATE A separate data POINTER
	}
}