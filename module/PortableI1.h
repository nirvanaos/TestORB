#ifndef NIRVANA_TESTORB_PORTABLEI1_H_
#define NIRVANA_TESTORB_PORTABLEI1_H_
#pragma once

#include "ImplI1.h"
#include <IDL/Test_I1_s.h>

namespace Test {

// Portable implementation

class PortableI1 :
	public virtual CORBA::servant_traits <I1>::base_type,
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

private:
	int32_t addendum_;
};

}

#endif
