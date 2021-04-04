#ifndef NIRVANA_TESTORB_DYNAMICI3_H_
#define NIRVANA_TESTORB_DYNAMICI3_H_

#include "ImplI1.h"
#include "Test_I3_s.h"

namespace Test {

// Dynamic implementation

class DynamicI3 :
	public CORBA::servant_traits <I3>::Servant <DynamicI3>,
	public ImplI1
{
public:
	DynamicI3 (CORBA::Long addendum) :
		addendum_ (addendum)
	{}

	CORBA::Long op1 (CORBA::Long p1) const
	{
		return p1 + addendum_;
	}

	static void throw_no_implement ()
	{
		throw CORBA::NO_IMPLEMENT ();
	}

	CORBA::Long op2 (CORBA::Long p1) const
	{
		return p1 + 2 * addendum_;
	}

	CORBA::Long op3 (CORBA::Long p1) const
	{
		return p1 + 3 * addendum_;
	}

private:
	CORBA::Long addendum_;
};

}

#endif
