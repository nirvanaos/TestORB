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
#ifndef NIRVANA_TESTORB_TEST_PORTABLEI2_H_
#define NIRVANA_TESTORB_TEST_PORTABLEI2_H_
#pragma once

#include "ImplI2.h"
#include "Test_I2_s.h"

namespace Test {

// Portable implementation of interface I2
class PortableI2 :
#ifdef LEGACY_CORBA_CPP
	public virtual POA_Test::I2,
#else
	public virtual CORBA::servant_traits <I2>::base_type,
#endif
	public ImplI2
{
public:
	PortableI2 (int32_t addendum) :
		addendum_ (addendum)
	{
	}

	virtual CORBA::Long op2 (CORBA::Long p1)
	{
		return p1 + 2 * addendum_;
	}

	virtual CORBA::Long divide (CORBA::Long a, CORBA::Long b)
	{
		return ImplI2::divide (a, b);
	}

	virtual CORBA::Long aop (CORBA::Long x)
	{
		return x - addendum_;
	}

#ifdef LEGACY_CORBA_CPP
	virtual A1::_ptr_type abstract_op (A1::_ptr_type in_obj, A1::_var_type& out_obj,
		A1::_var_type& inout_obj)
#else
	virtual A1::_ref_type abstract_op (A1::_ptr_type in_obj, A1::_ref_type& out_obj,
		A1::_ref_type& inout_obj)
#endif
	{
		return ImplI2::abstract_op (in_obj, out_obj, inout_obj);
	}

#ifdef LEGACY_CORBA_CPP
	virtual V1::_ptr_type value_op (V1::_ptr_type in_obj, V1::_var_type& out_obj,
		V1::_var_type& inout_obj)
#else
	virtual V1::_ref_type value_op (V1::_ptr_type in_obj, V1::_ref_type& out_obj,
		V1::_ref_type& inout_obj)
#endif
	{
		return ImplI2::value_op (in_obj, out_obj, inout_obj);
	}

#ifdef LEGACY_CORBA_CPP
	virtual StringValue::_ptr_type box_op (StringValue::_ptr_type in_obj, StringValue::_var_type& out_obj,
		StringValue::_var_type& inout_obj)
#else
	virtual StringValue::_ref_type box_op (StringValue::_ptr_type in_obj, StringValue::_ref_type& out_obj,
		StringValue::_ref_type& inout_obj)
#endif
	{
		return ImplI2::box_op (in_obj, out_obj, inout_obj);
	}

	virtual BoxArray box_array_op (const BoxArray& in_ar, BoxArray& out_ar, BoxArray& inout_ar)
	{
		return ImplI2::box_array_op (in_ar, out_ar, inout_ar);
	}

	virtual U union_op (const U& in_u, U& out_u, U& inout_u)
	{
		return ImplI2::union_op (in_u, out_u, inout_u);
	}

#ifdef LEGACY_CORBA_CPP
	virtual CORBA::TypeCode::_ptr_type type_code_op (CORBA::TypeCode::_ptr_type in_tc,
		CORBA::TypeCode::_var_type& out_tc, CORBA::TypeCode::_var_type& inout_tc)
#else
	virtual CORBA::TypeCode::_ref_type type_code_op (CORBA::TypeCode::_ptr_type in_tc,
		CORBA::TypeCode::_ref_type& out_tc, CORBA::TypeCode::_ref_type& inout_tc)
#endif
	{
		return ImplI2::type_code_op (in_tc, out_tc, inout_tc);
	}

	virtual void oneway_op (int32_t l)
	{}

private:
	int32_t addendum_;
};

}

#endif
