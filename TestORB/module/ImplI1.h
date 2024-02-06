/*
* Nirvana test suite.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#ifndef NIRVANA_TESTORB_TEST_IMPLI1_H_
#define NIRVANA_TESTORB_TEST_IMPLI1_H_
#pragma once

#include <CORBA/Server.h>
#include "Test_I1.h"

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
	
	static I1::_ptr_type object_op (I1::_ptr_type in_obj, I1::_var_type& out_obj, I1::_var_type& inout_obj)
	{
		I1::_var_type tmp (std::move (inout_obj));
		out_obj = I1::_duplicate (in_obj);
		inout_obj = I1::_duplicate (in_obj);
		return tmp._retn ();
	}

#else

	static I1::_ref_type object_op (I1::_ptr_type in_obj, I1::_ref_type& out_obj, I1::_ref_type& inout_obj)
	{
		I1::_ref_type tmp (std::move (inout_obj));
		out_obj = in_obj;
		inout_obj = in_obj;
		return tmp;
	}

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
	static float float_op (float in_f, float& out_f, float& inout_f);
	static CORBA::Double double_op (CORBA::Double in_f, CORBA::Double& out_f, CORBA::Double& inout_f);
	static CORBA::LongDouble long_double_op (const CORBA::LongDouble& in_f, CORBA::LongDouble& out_f, CORBA::LongDouble& inout_f);
};

}

#endif
