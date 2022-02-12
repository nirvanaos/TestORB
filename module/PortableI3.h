#ifndef NIRVANA_TESTORB_PORTABLEI3_H_
#define NIRVANA_TESTORB_PORTABLEI3_H_
#pragma once

#include "PortableI1.h"
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

	I3::_ref_type _this ()
	{
		return CORBA::servant_traits <I3>::base_type::_this ();
	}

	virtual CORBA::Long op1 (CORBA::Long p1)
	{
		return PortableI1::op1 (p1);
	}

	virtual void throw_no_implement ()
	{
		PortableI1::throw_no_implement ();
	}

	virtual void throw_user ()
	{
		PortableI1::throw_user ();
	}

	virtual I1::_ref_type object_op (I1::_ptr_type in_obj,
		I1::_ref_type& out_obj, I1::_ref_type& inout_obj)
	{
		return PortableI1::object_op (in_obj, out_obj, inout_obj);
	}

	virtual std::string string_op (const std::string& in_s,
		std::string& out_s, std::string& inout_s)
	{
		return PortableI1::string_op (in_s, out_s, inout_s);
	}

	virtual std::vector <CORBA::Long> seq_op (const std::vector <CORBA::Long>& in_s,
		std::vector <CORBA::Long>& out_s, std::vector <CORBA::Long>& inout_s)
	{
		return PortableI1::seq_op (in_s, out_s, inout_s);
	}

	virtual CORBA::Any any_op (const CORBA::Any& in_a, CORBA::Any& out_a,
		CORBA::Any& inout_a)
	{
		return PortableI1::any_op (in_a, out_a, inout_a);
	}

	virtual int32_t addendum ()
	{
		return PortableI1::addendum ();
	}

	virtual void addendum (int32_t a)
	{
		PortableI1::addendum (a);
	}

	virtual std::string short_string_op (const std::string& in_s,
		std::string& out_s, std::string& inout_s)
	{
		return PortableI1::short_string_op (in_s, out_s, inout_s);
	}

	virtual std::vector <CORBA::Long> short_seq_op (
		const std::vector <CORBA::Long>& in_s, std::vector <CORBA::Long>& out_s,
		std::vector <CORBA::Long>& inout_s)
	{
		return PortableI1::short_seq_op (in_s, out_s, inout_s);
	}

	virtual CORBA::Long op2 (CORBA::Long p1)
	{
		return p1 + 2 * addendum ();
	}

	virtual CORBA::Long op3 (CORBA::Long p1)
	{
		return p1 + 3 * addendum ();
	}
};

}

#endif
