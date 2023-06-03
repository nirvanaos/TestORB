#ifndef NIRVANA_TESTORB_DYNAMICI1_H_
#define NIRVANA_TESTORB_DYNAMICI1_H_
#pragma once

#include "ImplI1.h"
#include <IDL/Test_I1_s.h>

namespace Test {

// Dynamic implementation of the interface Test::I1

class DynamicI1 :
	public CORBA::servant_traits <I1>::Servant <DynamicI1>,
	public ImplI1
{
public:
	DynamicI1 (int32_t addendum) :
		addendum_ (addendum)
	{}

	~DynamicI1 ()
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

private:
	int32_t addendum_;
};

}

#endif
