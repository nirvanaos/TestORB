#ifndef NIRVANA_TESTORB_PORTABLEI2_H_
#define NIRVANA_TESTORB_PORTABLEI2_H_
#pragma once

#include "ImplI2.h"
#include <IDL/Test_I2_s.h>

namespace Test {

// Portable implementation of interface I2

class PortableI2 :
	public virtual CORBA::servant_traits <I2>::base_type,
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

private:
	int32_t addendum_;
};

}

#endif
