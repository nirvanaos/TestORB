#ifndef NIRVANA_TESTORB_DYNAMICI3_H_
#define NIRVANA_TESTORB_DYNAMICI3_H_
#pragma once

#include "ImplI1.h"
#include <IDL/Test_I3_s.h>
#include <IDL/Test_I2_s.h>
#include <IDL/Test_I1_s.h>

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

	~DynamicI3 ()
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

	static int32_t divide (int32_t a, int32_t b)
	{
		return a / b;
	}

private:
	int32_t addendum_;
};

}

#endif
