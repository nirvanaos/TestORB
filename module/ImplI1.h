#ifndef NIRVANA_TESTORB_IMPLI1_H_
#define NIRVANA_TESTORB_IMPLI1_H_
#pragma once

#include <CORBA/CORBA.h>
#include <IDL/Test_I1.h>

namespace Test {

class ImplI1
{
public:
	ImplI1 ()
	{}

	~ImplI1 ()
	{}

	static void throw_no_implement ();
	static void throw_user ();
	static I1::_ref_type object_op (I1::_ptr_type in_obj, I1::_ref_type& out_obj, I1::_ref_type& inout_obj);
	static std::string string_op (const std::string& in_s, std::string& out_s, std::string& inout_s);
	static std::vector <CORBA::Long> seq_op (const std::vector <CORBA::Long>& in_s, std::vector <CORBA::Long>& out_s, std::vector <CORBA::Long>& inout_s);
	static CORBA::Any any_op (const CORBA::Any& in_any, CORBA::Any& out_any, CORBA::Any& inout_any);
	static std::string short_string_op (const std::string& in_s, std::string& out_s, std::string& inout_s);
	static std::vector <CORBA::Long> short_seq_op (const std::vector <CORBA::Long>& in_s, std::vector <CORBA::Long>& out_s, std::vector <CORBA::Long>& inout_s);
	static std::vector <std::string> seq_string_op (const std::vector <std::string>& in_s, std::vector <std::string>& out_s, std::vector <std::string>& inout_s);
};

}

#endif
