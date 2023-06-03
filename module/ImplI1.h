#ifndef NIRVANA_TESTORB_IMPLI1_H_
#define NIRVANA_TESTORB_IMPLI1_H_
#pragma once

#include <CORBA/Server.h>
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
#ifdef LEGACY_CORBA_CPP
	static I1::_ptr_type object_op (I1::_ptr_type in_obj, I1::_var_type& out_obj, I1::_var_type& inout_obj);
#else
	static I1::_ref_type object_op (I1::_ptr_type in_obj, I1::_ref_type& out_obj, I1::_ref_type& inout_obj);
#endif
	static std::string string_op (const std::string& in_s, std::string& out_s, std::string& inout_s);
	static std::vector <CORBA::Long> seq_op (const std::vector <CORBA::Long>& in_s, std::vector <CORBA::Long>& out_s, std::vector <CORBA::Long>& inout_s);
	static CORBA::Any any_op (const CORBA::Any& in_any, CORBA::Any& out_any, CORBA::Any& inout_any);
	static std::string short_string_op (const std::string& in_s, std::string& out_s, std::string& inout_s);
	static std::vector <CORBA::Long> short_seq_op (const std::vector <CORBA::Long>& in_s, std::vector <CORBA::Long>& out_s, std::vector <CORBA::Long>& inout_s);
	static std::vector <std::string> seq_string_op (const std::vector <std::string>& in_s, std::vector <std::string>& out_s, std::vector <std::string>& inout_s);
	static MyStruct struct_op (const MyStruct& in_s, MyStruct& out_s, MyStruct& inout_s);
	static std::vector <MyStruct> seq_struct_op (const std::vector <MyStruct>& in_s, std::vector <MyStruct>& out_s, std::vector <MyStruct>& inout_s);
	static StructArray struct_array_op (const StructArray& in_ar, StructArray& out_ar, StructArray& inout_ar);
	static bool bool_op (bool in_b, bool& out_b, bool& inout_b);
	static Fixed_8_2 fixed_op (Fixed_8_2 in_f, Fixed_8_2& out_f, Fixed_8_2& inout_f);
	static StructFixLen fix_struct_op (const StructFixLen& in_s, StructFixLen& out_s, StructFixLen& inout_s);
	static LongArray long_array_op (const LongArray& in_ar, LongArray& out_ar, LongArray& inout_ar);
	static FixStructArray fix_struct_array_op (const FixStructArray& in_ar, FixStructArray& out_ar, FixStructArray& inout_ar);
};

}

#endif
