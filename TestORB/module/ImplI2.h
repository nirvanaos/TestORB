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
#ifndef NIRVANA_TESTORB_TEST_IMPLI2_H_
#define NIRVANA_TESTORB_TEST_IMPLI2_H_
#pragma once

#include <CORBA/Server.h>
#include "IDL/Test_I2.h"

namespace Test {

class ImplI2
{
public:
	ImplI2 ()
	{
	}

	~ImplI2 ()
	{
	}

	static int32_t divide (int32_t a, int32_t b);

#ifdef LEGACY_CORBA_CPP
	static A1::_ptr_type abstract_op (A1::_ptr_type in_obj, A1::_var_type& out_obj, A1::_var_type& inout_obj);
	static V1::_ptr_type value_op (V1::_ptr_type in_obj, V1::_var_type& out_obj, V1::_var_type& inout_obj);
	static StringValue::_ptr_type box_op (StringValue::_ptr_type in_obj, StringValue::_var_type& out_obj, StringValue::_var_type& inout_obj);
#else
	static A1::_ref_type abstract_op (A1::_ptr_type in_obj, A1::_ref_type& out_obj, A1::_ref_type& inout_obj);
	static V1::_ref_type value_op (V1::_ptr_type in_obj, V1::_ref_type& out_obj, V1::_ref_type& inout_obj);
	static StringValue::_ref_type box_op (StringValue::_ptr_type in_obj, StringValue::_ref_type& out_obj, StringValue::_ref_type& inout_obj);
#endif

	static BoxArray box_array_op (const BoxArray& in_ar, BoxArray& out_ar, BoxArray& inout_ar);
	static U union_op (const U& in_u, U& out_u, U& inout_u);

#ifdef LEGACY_CORBA_CPP
	static CORBA::TypeCode::_ptr_type type_code_op (CORBA::TypeCode::_ptr_type in_obj,
		CORBA::TypeCode::_var_type& out_obj, CORBA::TypeCode::_var_type& inout_obj);
#else
	static CORBA::TypeCode::_ref_type type_code_op (CORBA::TypeCode::_ptr_type in_obj,
		CORBA::TypeCode::_ref_type& out_obj, CORBA::TypeCode::_ref_type& inout_obj);
#endif

	static void oneway_op (long l)
	{}
};

}

#endif
