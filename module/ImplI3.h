#ifndef NIRVANA_TESTORB_IMPLI3_H_
#define NIRVANA_TESTORB_IMPLI3_H_
#pragma once

#include <CORBA/CORBA.h>
#include <IDL/Test_I3.h>

namespace Test {

class ImplI3
{
public:
	ImplI3 ()
	{}

	~ImplI3 ()
	{}

	static int32_t divide (int32_t a, int32_t b)
	{
		return a / b;
	}

#ifdef LEGACY_CORBA_CPP

	static A1::_ptr_type abstract_op (A1::_ptr_type in_obj, A1::_var_type& out_obj, A1::_var_type& inout_obj)
	{
		A1::_var_type tmp (std::move (inout_obj));
		out_obj = A1::_duplicate (in_obj);
		inout_obj = A1::_duplicate (in_obj);
		return tmp._retn ();
	}

#else

	static A1::_ref_type abstract_op (A1::_ptr_type in_obj, A1::_ref_type& out_obj, A1::_ref_type& inout_obj)
	{
		A1::_ref_type tmp (std::move (inout_obj));
		out_obj = in_obj;
		inout_obj = in_obj;
		return tmp;
	}

#endif

};

}

#endif
