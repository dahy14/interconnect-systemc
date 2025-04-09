#pragma once
#ifndef __COMP3__
#define __COMP3__

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
namespace TTT {
	class comp3 : sc_core::sc_module
	{
	public:
		SC_HAS_PROCESS(comp3);
		comp3(sc_core::sc_module_name name);
		tlm_utils::simple_target_socket<comp3> socks2;
		void comp3_b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay);
		enum { SIZE = 256 };
	private:
		int mem[SIZE];
	};

}
#endif

