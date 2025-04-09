#include "comp3.h"
#include <systemc>
#include <tlm>

namespace TTT {
	comp3::comp3(sc_core::sc_module_name name) :
		sc_core::sc_module(name),
		socks2("socks3") 
	{
		socks2.register_b_transport(this, &comp3::comp3_b_transport);
		for (int i = 0; i < SIZE; i++) 
		{
			mem[i] = 0xDEAD0000 | 0xBEEF;
		};
	}

	void comp3::comp3_b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay)
	{
		tlm::tlm_command	cmd = trans.get_command();
		sc_dt::uint64		adr = trans.get_address(); 
		unsigned char*		ptr = trans.get_data_ptr();
		unsigned int		len = trans.get_data_length();
		unsigned char*		byt = trans.get_byte_enable_ptr();
		unsigned int		wid = trans.get_streaming_width();

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
		};
		trans.set_response_status(tlm::TLM_OK_RESPONSE);
	}
}