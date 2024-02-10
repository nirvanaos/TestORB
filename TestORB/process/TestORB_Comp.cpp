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
#include <Test_Comp.h>
#include <Test_I1_factory.h>
#include <gtest/gtest.h>

using namespace CORBA;
using namespace Test;

namespace TestORB {

// The fixture for testing component interface.

#ifdef LEGACY_CORBA_CPP
typedef Comp_var Comp_ref;
typedef Components::Cookie_var Cookie_ref;
#else
typedef Comp::_ref_type Comp_ref;
typedef Components::Cookie::_ref_type Cookie_ref;
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

TEST_P (TestORB_Comp, Facet)
{
	Comp_ref p = instantiate ();
	I1_ref facet = p->provide_facet1 ();
	ASSERT_TRUE (facet);
	test_interface (facet);
}

TEST_P (TestORB_Comp, Receptacles)
{
	Comp_ref p = instantiate ();
	I1_ref obj1 = I1_factory_dynamic->create (MAGIC_CONST);
	p->connect_single (obj1);
	I1_ref obj2 = I1_factory_dynamic->create (MAGIC_CONST);
	Cookie_ref ck = p->connect_multi (obj2);

	auto conns = p->get_all_connections ();
	EXPECT_EQ (conns.size (), 2);
	I1_ref obj2_1 = p->disconnect_multi (ck);
	EXPECT_TRUE (obj2_1);
	EXPECT_TRUE (obj2->_is_equivalent (obj2_1));

	conns = p->get_all_connections ();
	EXPECT_EQ (conns.size (), 1);

	I1_ref obj1_1 = p->disconnect_single ();
	EXPECT_TRUE (obj1_1);
	EXPECT_TRUE (obj1->_is_equivalent (obj1_1));

	conns = p->get_all_connections ();
	EXPECT_EQ (conns.size (), 0);
}

TEST_P (TestORB_Comp, Navigation)
{
	Comp_ref p = instantiate ();
	Object_ref comp = p->_get_component ();
	EXPECT_TRUE (!comp);

	I1_ref facet = I1::_narrow (p->provide_facet ("facet1"));
	ASSERT_TRUE (facet);

	comp = facet->_get_component ();
	EXPECT_TRUE (comp);
	EXPECT_TRUE (comp->_is_equivalent (p));

	I1_ref obj1 = I1_factory_dynamic->create (MAGIC_CONST);
	Cookie_ref ck = p->connect ("single", obj1);
	EXPECT_FALSE (ck);
	I1_ref obj2 = I1_factory_dynamic->create (MAGIC_CONST);
	ck = p->connect ("multi", obj2);
	EXPECT_TRUE (ck);

	auto conns = p->get_all_connections ();
	EXPECT_EQ (conns.size (), 2);
	Object_ref obj2_1 = p->disconnect ("multi", ck);
	EXPECT_TRUE (obj2_1);
	EXPECT_TRUE (obj2->_is_equivalent (obj2_1));

	conns = p->get_all_connections ();
	EXPECT_EQ (conns.size (), 1);

	Object_ref obj1_1 = p->disconnect ("single", nullptr);
	EXPECT_TRUE (obj1_1);
	EXPECT_TRUE (obj1->_is_equivalent (obj1_1));

	conns = p->get_all_connections ();
	EXPECT_EQ (conns.size (), 0);
}

}
