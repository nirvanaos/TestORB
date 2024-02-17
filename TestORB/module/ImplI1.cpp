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
#include "pch.h"
#include "ImplI1.h"

using namespace CORBA;

namespace Test {

void ImplI1::throw_no_implement ()
{
	throw NO_IMPLEMENT ();
}

void ImplI1::throw_user ()
{
	std::exception_ptr ep;
	try {
		throw MyException ("test", true);
	} catch (...) {
		ep = std::current_exception ();
	}

	std::rethrow_exception (ep);
}

std::string ImplI1::string_op (const std::string& in_s, std::string& out_s, std::string& inout_s)
{
	std::string tmp (std::move (inout_s));
	out_s = in_s;
	inout_s = in_s;
	return tmp;
}

std::vector <Long> ImplI1::seq_op (const std::vector <Long>& in_s, std::vector <Long>& out_s, std::vector <Long>& inout_s)
{
	std::vector <Long> tmp (std::move (inout_s));
	out_s = in_s;
	inout_s = in_s;
	return tmp;
}

CORBA::Any ImplI1::any_op (const CORBA::Any& in_any, CORBA::Any& out_any, CORBA::Any& inout_any)
{
	CORBA::Any tmp (std::move (inout_any));
	out_any = in_any;
	inout_any = in_any;
	return tmp;
}

std::string ImplI1::short_string_op (const std::string& in_s, std::string& out_s, std::string& inout_s)
{
	return string_op (in_s, out_s, inout_s);
}

std::vector <Long> ImplI1::short_seq_op (const std::vector <Long>& in_s,
	std::vector <Long>& out_s, std::vector <Long>& inout_s)
{
	return seq_op (in_s, out_s, inout_s);
}

std::vector <std::string> ImplI1::seq_string_op (
	const std::vector <std::string>& in_s, std::vector <std::string>& out_s,
	std::vector <std::string>& inout_s)
{
	std::vector <std::string> tmp (std::move (inout_s));
	out_s = in_s;
	inout_s = in_s;
	return tmp;
}

MyStruct ImplI1::struct_op (const MyStruct& in_s, MyStruct& out_s, MyStruct& inout_s)
{
	MyStruct tmp (std::move (inout_s));
	out_s = in_s;
	inout_s = in_s;
	return tmp;
}

std::vector <MyStruct> ImplI1::seq_struct_op (
	const std::vector <MyStruct>& in_s, std::vector <MyStruct>& out_s,
	std::vector <MyStruct>& inout_s)
{
	std::vector <MyStruct> tmp (std::move (inout_s));
	out_s = in_s;
	inout_s = in_s;
	return tmp;
}

StructArray ImplI1::struct_array_op (const StructArray& in_ar, StructArray& out_ar, StructArray& inout_ar)
{
	StructArray tmp (inout_ar);
	out_ar = in_ar;
	inout_ar = in_ar;
	return tmp;
}

bool ImplI1::bool_op (bool in_b, bool& out_b, bool& inout_b)
{
	bool tmp = inout_b;
	out_b = in_b;
	inout_b = in_b;
	return tmp;
}

Fixed_8_2 ImplI1::fixed_op (Fixed_8_2 in_f, Fixed_8_2& out_f, Fixed_8_2& inout_f)
{
	Fixed_8_2 tmp = inout_f;
	out_f = in_f;
	inout_f = in_f;
	return tmp;
}

StructFixLen ImplI1::fix_struct_op (const StructFixLen& in_s, StructFixLen& out_s, StructFixLen& inout_s)
{
	StructFixLen tmp (inout_s);
	out_s = in_s;
	inout_s = in_s;
	return tmp;
}

LongArray ImplI1::long_array_op (const LongArray& in_ar, LongArray& out_ar, LongArray& inout_ar)
{
	LongArray tmp (inout_ar);
	out_ar = in_ar;
	inout_ar = in_ar;
	return tmp;
}

FixStructArray ImplI1::fix_struct_array_op (const FixStructArray& in_ar, FixStructArray& out_ar, FixStructArray& inout_ar)
{
	FixStructArray tmp (inout_ar);
	out_ar = in_ar;
	inout_ar = in_ar;
	return tmp;
}

float ImplI1::float_op (float in_f, float& out_f, float& inout_f)
{
	float tmp = inout_f;
	out_f = in_f;
	inout_f = in_f;
	return tmp;
}

Double ImplI1::double_op (const Double in_f, Double& out_f, Double& inout_f)
{
	Double tmp = inout_f;
	out_f = in_f;
	inout_f = in_f;
	return tmp;
}

LongDouble ImplI1::long_double_op (const LongDouble& in_f, LongDouble& out_f, LongDouble& inout_f)
{
	LongDouble tmp = inout_f;
	out_f = in_f;
	inout_f = in_f;
	return tmp;
}

}