#pragma once 
#ifndef __COMP2__
#define __COMP2__

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

namespace TTT {
	class comp2 : sc_core::sc_module
	{
	public:
		SC_HAS_PROCESS(comp2);
		comp2(sc_core::sc_module_name name);
		
		// for target socket
		tlm_utils::simple_target_socket<comp2> socks1; 
		virtual void comp2_b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay);
		enum { SIZE = 256 };

		// for initiator socket
		tlm_utils::simple_initiator_socket<comp2> socks2;
		void thread_process();		
		tlm::tlm_command	cmd;
		sc_dt::uint64		adr;
		unsigned char*		ptr;
		unsigned int		len;
		unsigned char*		byt;
		unsigned int		wid;
	private:
		int mem[SIZE]; 
		
		tlm::tlm_generic_payload *trans_ptr; 
	};
}

#endif