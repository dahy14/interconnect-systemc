#pragma once
#ifndef __COMP1_
#define __COMP1__

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>

namespace TTT {
	class comp1 : sc_core::sc_module
	{
	public:
		SC_HAS_PROCESS(comp1);
		comp1(sc_core::sc_module_name name);
		tlm_utils::simple_initiator_socket<comp1> socks1;
		void thread_process(); 
		void thread_process2();
	private: 
		sc_dt::uint64 data;
		sc_core::sc_event write_done_event;
	};
}
#endif