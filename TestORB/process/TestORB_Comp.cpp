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
#include "TestORB_I1.h"
#include <I1_static.h>
#include "Test_Comp.h"
#include <gtest/gtest.h>

using namespace CORBA;
using namespace Test;

namespace TestORB {

// The fixture for testing component interface.

#ifdef LEGACY_CORBA_CPP
typedef Comp_var Comp_ref;
#else
typedef Comp::_ref_type Comp_ref;
#endif

typedef Comp_ref (*Comp_Factory) ();

class TestORB_Comp :
	public ::testing::TestWithParam <Comp_Factory>
{
protected:
	TestORB_Comp ()
	{}

	virtual ~TestORB_Comp ()
	{}

	Comp_ref instantiate ()
	{
		return (GetParam ()) ();
	}
};

Comp_ref Comp_get_dynamic ()
{
	return comp_factory_dynamic->create (MAGIC_CONST);
}

INSTANTIATE_TEST_SUITE_P (ServantTypesComp, TestORB_Comp, testing::Values (
	Comp_get_dynamic      // 0
));

TEST_P (TestORB_Comp, Interface)
{
	Comp_ref p = instantiate ();
	I1_ref facet = p->provide_facet1 ();
	test_interface (facet);
}

}
