#ifndef NIRVANA_TESTORB_PORTABLEI3_H_
#define NIRVANA_TESTORB_PORTABLEI3_H_
#pragma once

#include "PortableI1.h"
#include "ImplI3.h"
#include <IDL/Test_I3_s.h>

namespace Test {

// Portable implementation

class PortableI3 :
	public virtual CORBA::servant_traits <I3>::base_type,
	public virtual PortableI1 // Implementation inheritance
{
public:
	PortableI3 (CORBA::Long addendum) :
		PortableI1 (addendum)
	{}

	virtual CORBA::Long op2 (CORBA::Long p1)
	{
		return p1 + 2 * addendum ();
	}

	virtual CORBA::Long op3 (CORBA::Long p1)
	{
		return p1 + 3 * addendum ();
	}

	virtual CORBA::Long divide (CORBA::Long a, CORBA::Long b)
	{
		return ImplI3::divide (a, b);
	}

	virtual CORBA::Long aop (CORBA::Long x)
	{
		return x - addendum ();
	}

#ifdef LEGACY_CORBA_CPP
	virtual A1::_ptr_type abstract_op (A1::_ptr_type in_obj, A1::_var_type& out_obj,
		A1::_var_type& inout_obj)
#else
	virtual A1::_ref_type abstract_op (A1::_ptr_type in_obj, A1::_ref_type& out_obj,
		A1::_ref_type& inout_obj)
#endif
	{
		return ImplI3::abstract_op (in_obj, out_obj, inout_obj);
	}

#ifdef LEGACY_CORBA_CPP
	virtual V1::_ptr_type value_op (V1::_ptr_type in_obj, V1::_var_type& out_obj,
		V1::_var_type& inout_obj)
#else
	virtual V1::_ref_type value_op (V1::_ptr_type in_obj, V1::_ref_type& out_obj,
		V1::_ref_type& inout_obj)
#endif
	{
		return ImplI3::value_op (in_obj, out_obj, inout_obj);
	}

	virtual StructFixLen fix_struct_op (const StructFixLen& in_s, StructFixLen& out_s, StructFixLen& inout_s)
	{
		return ImplI3::fix_struct_op (in_s, out_s, inout_s);
	}

	virtual LongArray long_array_op (const LongArray& in_ar, LongArray& out_ar, LongArray& inout_ar)
	{
		return ImplI3::long_array_op (in_ar, out_ar, inout_ar);
	}

	virtual StructArray struct_array_op (const StructArray& in_ar, StructArray& out_ar, StructArray& inout_ar)
	{
		return ImplI3::struct_array_op (in_ar, out_ar, inout_ar);
	}

	virtual FixStructArray fix_struct_array_op (const FixStructArray& in_ar, FixStructArray& out_ar, FixStructArray& inout_ar)
	{
		return ImplI3::fix_struct_array_op (in_ar, out_ar, inout_ar);
	}

#ifdef LEGACY_CORBA_CPP
	virtual StringValue::_ptr_type box_op (StringValue::_ptr_type in_obj, StringValue::_var_type& out_obj,
		StringValue::_var_type& inout_obj)
#else
	virtual StringValue::_ref_type box_op (StringValue::_ptr_type in_obj, StringValue::_ref_type& out_obj,
		StringValue::_ref_type& inout_obj)
#endif
	{
		return ImplI3::box_op (in_obj, out_obj, inout_obj);
	}

	virtual BoxArray box_array_op (const BoxArray& in_ar, BoxArray& out_ar, BoxArray& inout_ar)
	{
		return ImplI3::box_array_op (in_ar, out_ar, inout_ar);
	}

	virtual U union_op (const U& in_u, U& out_u, U& inout_u)
	{
		return ImplI3::union_op (in_u, out_u, inout_u);
	}

	virtual bool bool_op (bool in_b, bool& out_b, bool& inout_b)
	{
		return ImplI3::bool_op (in_b, out_b, inout_b);
	}

	virtual Fixed_8_2 fixed_op (const Fixed_8_2& in_f, Fixed_8_2& out_f, Fixed_8_2& inout_f)
	{
		return ImplI3::fixed_op (in_f, out_f, inout_f);
	}
};

}

#endif
