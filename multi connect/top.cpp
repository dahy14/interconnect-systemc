#include "top.h"
#include <systemc>
#include <tlm>


Top::Top(sc_core::sc_module_name name) : sc_core::sc_module(name)
{
	COMP1 = new TTT::comp1("comp1");
	COMP2 = new TTT::comp2("comp2");
	COMP3 = new TTT::comp3("comp3");

	COMP1->socks1.bind(COMP2->socks1);
	COMP2->socks2.bind(COMP3->socks2);
};