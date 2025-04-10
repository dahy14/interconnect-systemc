#include "comp1.h"
#include <systemc>
#include <tlm>

namespace TTT {

	void comp1::thread_process() 
	{
		tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
		sc_core::sc_time delay = sc_core::sc_time(10, sc_core::SC_NS);
	
		// payload declaration or decoding if there is dynamic input

		sc_dt::uint64 i = 0x10;
		data = 0xAC12; // for write operation

		std::cout << " ------------------- ORIGINAL POINTER FROM THE SOURCE -------------------\n"
			<< "POINTER: " << std::hex << &data << ",\n"
			<< "  VALUE: " << std::hex << data
			<< "\n" << std::endl;

		tlm::tlm_command cmd = tlm::TLM_WRITE_COMMAND;
		trans->set_command(cmd);
		trans->set_address(i); // address size on target.h
		trans->set_data_ptr(reinterpret_cast<unsigned char*>(&data));
		trans->set_data_length(4);
		trans->set_streaming_width(4); // = data_length to indicate no streaming
		trans->set_byte_enable_ptr(0); // 0 indicates unused
		trans->set_dmi_allowed(false); // Mandatory initial value
		trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE); // Mandatory initial value
		socks1->b_transport(*trans, delay);
	
		if (trans->is_response_error())
			SC_REPORT_ERROR("TLM-2", "Response error from b_transport");
		wait(delay); 
		write_done_event.notify();
		
	};

	void comp1::thread_process2()
	{
		wait(write_done_event);
		tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
		sc_core::sc_time delay = sc_core::sc_time(10, sc_core::SC_NS);

		// payload declaration or decoding if there is dynamic input

		sc_dt::uint64 i = 0x10;
		sc_dt::uint64 data2 = 0x00000000; // for read operation

		std::cout << " ------------------- ORIGINAL POINTER FROM THE SOURCE -------------------\n"
			<< "POINTER: " << std::hex << &data2 << ",\n"
			<< "  VALUE: " << std::hex << data2
			<< "\n" << std::endl;

		tlm::tlm_command cmd = tlm::TLM_READ_COMMAND;
		trans->set_command(cmd);
		trans->set_address(i); // address size on target.h
		trans->set_data_ptr(reinterpret_cast<unsigned char*>(&data2));
		trans->set_data_length(4);
		trans->set_streaming_width(4); // = data_length to indicate no streaming
		trans->set_byte_enable_ptr(0); // 0 indicates unused
		trans->set_dmi_allowed(false); // Mandatory initial value
		trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE); // Mandatory initial value
		

		socks1->b_transport(*trans, delay);

		if (trans->is_response_error())
			SC_REPORT_ERROR("TLM-2", "Response error from b_transport");
		wait(delay);
	};


	comp1::comp1(sc_core::sc_module_name name) :
		sc_core::sc_module(name),
		socks1("socks1")
	{
		SC_THREAD(thread_process);
		SC_THREAD(thread_process2);
	};
}