#include <Nirvana/Nirvana.h>
#include "ImplI1.h"

namespace Test {

using namespace CORBA;

void ImplI1::throw_no_implement ()
{
	throw CORBA::NO_IMPLEMENT ();
}

void ImplI1::throw_user ()
{
	throw MyException ("test", true);
}

#ifdef LEGACY_CORBA_CPP

I1::_ptr_type ImplI1::object_op (I1::_ptr_type in_obj, I1::_var_type& out_obj, I1::_var_type& inout_obj)
{
	I1::_var_type tmp (std::move (inout_obj));
	out_obj = I1::_duplicate (in_obj);
	inout_obj = I1::_duplicate (in_obj);
	return tmp._retn ();
}

#else

I1::_ref_type ImplI1::object_op (I1::_ptr_type in_obj, I1::_ref_type& out_obj, I1::_ref_type& inout_obj)
{
	I1::_ref_type tmp (std::move (inout_obj));
	out_obj = in_obj;
	inout_obj = in_obj;
	return tmp;
}

#endif

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

}