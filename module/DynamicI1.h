#ifndef NIRVANA_TESTORB_DYNAMICI1_H_
#define NIRVANA_TESTORB_DYNAMICI1_H_

#include "ImplI1.h"
#include "Test_I1_s.h"

namespace Test {

// Dynamic implementation

class DynamicI1 :
	public CORBA::servant_traits <I1>::Servant <DynamicI1>,
	public ImplI1
{
public:
	DynamicI1 (CORBA::Long addendum) :
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

private:
	CORBA::Long addendum_;
};

}

#endif
