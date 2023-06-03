#ifndef NIRVANA_TESTORB_DYNAMICI2_H_
#define NIRVANA_TESTORB_DYNAMICI2_H_
#pragma once

#include "ImplI2.h"
#include <IDL/Test_I2_s.h>

namespace Test {

// Dynamic implementation of the interface Test::I2

class DynamicI2 :
	public CORBA::servant_traits <I2>::Servant <DynamicI2>,
	public ImplI2
{
public:
	DynamicI2 (int32_t addendum) :
		addendum_ (addendum)
	{
	}

	~DynamicI2 ()
	{
	}

	// abstract interface A1::aop()

	int32_t aop (int32_t x) const
	{
		return x - addendum_;
	}

	// interface I2

	int32_t op2 (int32_t p1) const
	{
		return p1 + 2 * addendum_;
	}

private:
	int32_t addendum_;
};

}

#endif
