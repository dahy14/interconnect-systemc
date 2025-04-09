#pragma once
#ifndef __TOP__
#define __TOP__

#include <systemc>
#include <tlm>
#include "comp1.h"
#include "comp2.h"
#include "comp3.h"


namespace TOP {
	class comp1;
	class comp2;
	class comp3;
};

class Top : sc_core::sc_module
{
public:
	SC_HAS_PROCESS(Top); 
	Top(sc_core::sc_module_name name);
	TTT::comp1* COMP1;
	TTT::comp2* COMP2;
	TTT::comp3* COMP3;

};

#endif