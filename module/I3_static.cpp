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
#include <CORBA/Server.h>
#include "I1_static.h"
#include "I3_static.h"
#include "ImplI1.h"
#include "ImplI2.h"
#include "ImplI3.h"
#include <IDL/Test_I1_s.h>
#include <IDL/Test_I2_s.h>
#include <IDL/Test_I3_s.h>

using namespace CORBA;

namespace Test {

// Static implementation of local interface I3
class I3_static :
	public servant_traits <I3>::ServantStatic <I3_static>,
	public ImplI1,
	public ImplI2,
	public ImplI3
{
public:
	static CORBA::Long op1 (CORBA::Long p1)
	{
		return p1 + TestORB::MAGIC_CONST;
	}

	static int32_t addendum ()
	{
		return TestORB::MAGIC_CONST;
	}

	void addendum (int32_t)
	{
		throw CORBA::NO_IMPLEMENT ();
	}

	static CORBA::Long op2 (CORBA::Long p1)
	{
		return p1 + 2 * TestORB::MAGIC_CONST;
	}

	static CORBA::Long op3 (CORBA::Long p1)
	{
		return p1 + 3 * TestORB::MAGIC_CONST;
	}

	static CORBA::Long aop (CORBA::Long x)
	{
		return x - TestORB::MAGIC_CONST;
	}
};

}

NIRVANA_STATIC_EXP (Test, I3_static)
