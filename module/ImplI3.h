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

#ifdef LEGACY_CORBA_CPP

	static V1::_ptr_type value_op (V1::_ptr_type in_obj, V1::_var_type& out_obj, V1::_var_type& inout_obj)
	{
		V1::_var_type tmp (std::move (inout_obj));
		out_obj = V1::_duplicate (in_obj);
		inout_obj = V1::_duplicate (in_obj);
		return tmp._retn ();
	}

#else

	static V1::_ref_type value_op (V1::_ptr_type in_obj, V1::_ref_type& out_obj, V1::_ref_type& inout_obj)
	{
		V1::_ref_type tmp (std::move (inout_obj));
		out_obj = in_obj;
		inout_obj = in_obj;
		return tmp;
	}

#endif

	static StructFixLen fix_struct_op (const StructFixLen& in_s, StructFixLen& out_s, StructFixLen& inout_s)
	{
		StructFixLen tmp (inout_s);
		out_s = in_s;
		inout_s = in_s;
		return tmp;
	}

	static LongArray long_array_op (const LongArray& in_ar, LongArray& out_ar, LongArray& inout_ar)
	{
		LongArray tmp (inout_ar);
		out_ar = in_ar;
		inout_ar = in_ar;
		return tmp;
	}

	static StructArray struct_array_op (const StructArray& in_ar, StructArray& out_ar, StructArray& inout_ar)
	{
		StructArray tmp (inout_ar);
		out_ar = in_ar;
		inout_ar = in_ar;
		return tmp;
	}

	static FixStructArray fix_struct_array_op (const FixStructArray& in_ar, FixStructArray& out_ar, FixStructArray& inout_ar)
	{
		FixStructArray tmp (inout_ar);
		out_ar = in_ar;
		inout_ar = in_ar;
		return tmp;
	}

#ifdef LEGACY_CORBA_CPP

	static StringValue::_ptr_type box_op (StringValue::_ptr_type in_obj, StringValue::_var_type& out_obj, StringValue::_var_type& inout_obj)
	{
		StringValue::_var_type tmp (std::move (inout_obj));
		out_obj = StringValue::_duplicate (in_obj);
		inout_obj = StringValue::_duplicate (in_obj);
		return tmp._retn ();
	}

#else

	static StringValue::_ref_type box_op (StringValue::_ptr_type in_obj, StringValue::_ref_type& out_obj, StringValue::_ref_type& inout_obj)
	{
		StringValue::_ref_type tmp (std::move (inout_obj));
		out_obj = in_obj;
		inout_obj = in_obj;
		return tmp;
	}

#endif

};

}

#endif
