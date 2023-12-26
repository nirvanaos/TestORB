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
#ifndef NIRVANA_TESTORB_TEST_PORTABLEI1_H_
#define NIRVANA_TESTORB_TEST_PORTABLEI1_H_
#pragma once

#include "ImplI1.h"
#include "Test_I1_s.h"

namespace Test {

// Portable implementation of interface I1
class PortableI1 :
#ifdef LEGACY_CORBA_CPP
	public virtual POA_Test::I1,
#else
	public virtual CORBA::servant_traits <I1>::base_type,
#endif
	public ImplI1
{
public:
	PortableI1 (int32_t addendum) :
		addendum_ (addendum)
	{}

	virtual int32_t op1 (int32_t p1)
	{
		return p1 + addendum_;
	}

	virtual void throw_no_implement ()
	{
		ImplI1::throw_no_implement ();
	}

	virtual void throw_user ()
	{
		ImplI1::throw_user ();
	}

#ifdef LEGACY_CORBA_CPP
	virtual I1::_ptr_type object_op (I1::_ptr_type in_obj, I1::_var_type& out_obj,
		I1::_var_type& inout_obj)
#else
	virtual I1::_ref_type object_op (I1::_ptr_type in_obj, I1::_ref_type& out_obj,
		I1::_ref_type& inout_obj)
#endif
	{
		return ImplI1::object_op (in_obj, out_obj, inout_obj);
	}

	virtual std::string string_op (const std::string& in_s, std::string& out_s,
		std::string& inout_s)
	{
		return ImplI1::string_op (in_s, out_s, inout_s);
	}

	virtual std::vector <CORBA::Long> seq_op (const std::vector <CORBA::Long>& in_s,
		std::vector <CORBA::Long>& out_s, std::vector <CORBA::Long>& inout_s)
	{
		return ImplI1::seq_op (in_s, out_s, inout_s);
	}

	virtual CORBA::Any any_op (const CORBA::Any& in_a, CORBA::Any& out_a,
		CORBA::Any& inout_a)
	{
		return ImplI1::any_op (in_a, out_a, inout_a);
	}

	virtual int32_t addendum ()
	{
		return addendum_;
	}

	virtual void addendum (int32_t a)
	{
		addendum_ = a;
	}

	virtual std::string short_string_op (const std::string& in_s,
		std::string& out_s, std::string& inout_s)
	{
		return ImplI1::short_string_op (in_s, out_s, inout_s);
	}

	virtual std::vector <CORBA::Long> short_seq_op (
		const std::vector <CORBA::Long>& in_s, std::vector <CORBA::Long>& out_s,
		std::vector <CORBA::Long>& inout_s)
	{
		return ImplI1::short_seq_op (in_s, out_s, inout_s);
	}

	virtual std::vector <std::string> seq_string_op (
		const std::vector <std::string>& in_s, std::vector <std::string>& out_s,
		std::vector <std::string>& inout_s)
	{
		return ImplI1::seq_string_op (in_s, out_s, inout_s);
	}

	virtual MyStruct struct_op (const MyStruct& in_s, MyStruct& out_s, MyStruct& inout_s)
	{
		return ImplI1::struct_op (in_s, out_s, inout_s);
	}

	virtual std::vector <MyStruct> seq_struct_op (const std::vector <MyStruct>& in_s, std::vector <MyStruct>& out_s, std::vector <MyStruct>& inout_s)
	{
		return ImplI1::seq_struct_op (in_s, out_s, inout_s);
	}

	virtual StructArray struct_array_op (const StructArray& in_ar, StructArray& out_ar, StructArray& inout_ar)
	{
		return ImplI1::struct_array_op (in_ar, out_ar, inout_ar);
	}

	virtual bool bool_op (bool in_b, bool& out_b, bool& inout_b)
	{
		return ImplI1::bool_op (in_b, out_b, inout_b);
	}

	virtual Fixed_8_2 fixed_op (const Fixed_8_2& in_f, Fixed_8_2& out_f, Fixed_8_2& inout_f)
	{
		return ImplI1::fixed_op (in_f, out_f, inout_f);
	}

	virtual StructFixLen fix_struct_op (const StructFixLen& in_s, StructFixLen& out_s, StructFixLen& inout_s)
	{
		return ImplI1::fix_struct_op (in_s, out_s, inout_s);
	}

	virtual LongArray long_array_op (const LongArray& in_ar, LongArray& out_ar, LongArray& inout_ar)
	{
		return ImplI1::long_array_op (in_ar, out_ar, inout_ar);
	}

	virtual FixStructArray fix_struct_array_op (const FixStructArray& in_ar, FixStructArray& out_ar, FixStructArray& inout_ar)
	{
		return ImplI1::fix_struct_array_op (in_ar, out_ar, inout_ar);
	}

private:
	int32_t addendum_;
};

}

#endif
