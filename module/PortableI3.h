#ifndef NIRVANA_TESTORB_PORTABLEI3_H_
#define NIRVANA_TESTORB_PORTABLEI3_H_
#pragma once

#include "PortableI1.h"
#include "PortableI2.h"
#include "ImplI3.h"
#include <IDL/Test_I3_s.h>

namespace Test {

// Portable implementation of local interface I3

class PortableI3 :
	public virtual CORBA::servant_traits <I3>::base_type,
	public virtual PortableI1, // Implementation inheritance
	public virtual PortableI2 // Implementation inheritance
{
public:
	PortableI3 (CORBA::Long addendum) :
		PortableI1 (addendum),
		PortableI2 (addendum)
	{}

	virtual CORBA::Long op3 (CORBA::Long p1)
	{
		return p1 + 3 * addendum ();
	}

};

}

#endif
