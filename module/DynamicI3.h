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
	DynamicI3 (int32_t addendum) :
		addendum_ (addendum)
	{}

	int32_t op1 (int32_t p1) const
	{
		return p1 + addendum_;
	}

	int32_t addendum () const
	{
		return addendum_;
	}

	void addendum (int32_t a)
	{
		addendum_ = a;
	}

	int32_t op2 (int32_t p1) const
	{
		return p1 + 2 * addendum_;
	}

	int32_t op3 (int32_t p1) const
	{
		return p1 + 3 * addendum_;
	}

private:
	int32_t addendum_;
};

}

#endif
