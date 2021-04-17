#ifndef NIRVANA_TESTORB_PORTABLEI3_H_
#define NIRVANA_TESTORB_PORTABLEI3_H_

#include "ImplI1.h"
#include "Test_I3_s.h"

namespace Test {

// Portable implementation

class PortableI3 :
	public POA_Test::I3,
	public ImplI1
{
public:
	PortableI3 (CORBA::Long addendum) :
		addendum_ (addendum)
	{}

	virtual CORBA::Long op1 (CORBA::Long p1)
	{
		return p1 + addendum_;
	}

	virtual void throw_no_implement ()
	{
		throw CORBA::NO_IMPLEMENT ();
	}

	virtual I1_var object_op (I1_ptr in_obj, I1_var& out_obj, I1_var& inout_obj)
	{
		return ImplI1::object_op (in_obj, out_obj, inout_obj);
	}

	virtual std::string string_op (const std::string& in_s, std::string& out_s, std::string& inout_s)
	{
		return ImplI1::string_op (in_s, out_s, inout_s);
	}

	virtual std::vector <CORBA::Long> seq_op (const std::vector <CORBA::Long>& in_s, std::vector <CORBA::Long>& out_s, std::vector <CORBA::Long>& inout_s)
	{
		return ImplI1::seq_op (in_s, out_s, inout_s);
	}

	virtual CORBA::Any any_op (const CORBA::Any& in_a, CORBA::Any& out_a, CORBA::Any& inout_a)
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

	virtual CORBA::Long op2 (CORBA::Long p1)
	{
		return p1 + 2 * addendum_;
	}

	virtual CORBA::Long op3 (CORBA::Long p1)
	{
		return p1 + 3 * addendum_;
	}

private:
	CORBA::Long addendum_;
};

}

#endif
