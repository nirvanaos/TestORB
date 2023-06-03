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

private:
	int32_t addendum_;
};

}

#endif
