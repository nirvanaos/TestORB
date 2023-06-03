#ifndef NIRVANA_TESTORB_IMPLI2_H_
#define NIRVANA_TESTORB_IMPLI2_H_
#pragma once

#include <CORBA/Server.h>
#include <IDL/Test_I2.h>

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
		V1::_var_type tmp = V1::_duplicate (V1::_downcast (inout_obj->_copy_value ()));
		out_obj = V1::_duplicate (in_obj);
		inout_obj = V1::_duplicate (in_obj);
		return tmp._retn ();
	}

#else

	static V1::_ref_type value_op (V1::_ptr_type in_obj, V1::_ref_type& out_obj, V1::_ref_type& inout_obj)
	{
		V1::_ref_type tmp = V1::_downcast (inout_obj->_copy_value ());
		inout_obj = nullptr;
		out_obj = in_obj;
		inout_obj = in_obj;
		return tmp;
	}

#endif

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

	static BoxArray box_array_op (const BoxArray& in_ar, BoxArray& out_ar, BoxArray& inout_ar)
	{
		BoxArray tmp (std::move (inout_ar));
		out_ar = in_ar;
		inout_ar = in_ar;
		return tmp;
	}

	static U union_op (const U& in_u, U& out_u, U& inout_u)
	{
		U tmp (inout_u);
		out_u = in_u;
		inout_u = in_u;
		return tmp;
	}
};

}

#endif
