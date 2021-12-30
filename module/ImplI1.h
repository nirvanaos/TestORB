#ifndef NIRVANA_TESTORB_IMPLI1_H_
#define NIRVANA_TESTORB_IMPLI1_H_

#include <CORBA/CORBA.h>
#include "Test_I1.h"

namespace Test {

class ImplI1
{
public:
	ImplI1 ()
	{}

	~ImplI1 ()
	{}

	static void throw_no_implement ()
	{
		throw CORBA::NO_IMPLEMENT ();
	}

	static void throw_user ()
	{
		throw MyException ("test", true);
	}

	static I1::_ref_type object_op (I1::_ptr_type in_obj, I1::_ref_type& out_obj, I1::_ref_type& inout_obj);
	static std::string string_op (const std::string& in_s, std::string& out_s, std::string& inout_s);
	static std::vector <CORBA::Long> seq_op (const std::vector <CORBA::Long>& in_s, std::vector <CORBA::Long>& out_s, std::vector <CORBA::Long>& inout_s);
	static CORBA::Any any_op (const CORBA::Any& in_any, CORBA::Any& out_any, CORBA::Any& inout_any);
};

}

#endif
