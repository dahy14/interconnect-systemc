#include "comp1.h"
#include <systemc>
#include <tlm>

namespace TTT {

	void comp1::thread_process() {
		std::cout << "comp1::thread_process" << std::endl;
		tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
		sc_core::sc_time delay = sc_core::sc_time(10, sc_core::SC_NS);
	
		// payload declaration or decoding if there is dynamic input
		int i = 0x0100;
		data = 0X21; // for write operations
		sc_dt::uint64 data2 = 0x65;
		tlm::tlm_command cmd = tlm::TLM_WRITE_COMMAND;
		trans->set_command(cmd);
		trans->set_address(i); // address size on target.h
		trans->set_data_ptr(reinterpret_cast<unsigned char*>(&data));
		trans->set_data_ptr(reinterpret_cast<unsigned char*>(&data2));
		trans->set_data_length(2);
		trans->set_streaming_width(2); // = data_length to indicate no streaming
		trans->set_byte_enable_ptr(0); // 0 indicates unused
		trans->set_dmi_allowed(false); // Mandatory initial value
		trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE); // Mandatory initial value
		std::cout << "data_ptr"<< trans->get_data_ptr() << std::endl;
		std::cout << "b_transport started" << std::endl;
		socks1->b_transport(*trans, delay);
		std::cout << "b_transport ended" << std::endl;
 
		std::cout << "trans = { " << cmd << ", " << std::hex << i
			<< " } , data = " << std::hex << data << " at time " << sc_core::sc_time_stamp()
			<< " delay = " << delay << "\n" << std::endl;

		if (trans->is_response_error())
			SC_REPORT_ERROR("TLM-2", "Response error from b_transport");
		wait(delay); 
	};

	comp1::comp1(sc_core::sc_module_name name) :
		sc_core::sc_module(name),
		socks1("socks1")
	{
		SC_THREAD(thread_process);
	};
}