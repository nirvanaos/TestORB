/*
* Nirvana test suite.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
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
#ifdef LEGACY_CORBA_CPP
	public virtual POA_Test::I3,
#else
	public virtual CORBA::servant_traits <I3>::base_type,
#endif
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
