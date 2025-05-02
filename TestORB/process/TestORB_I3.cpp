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
#include "pch.h"
#include "TestORB_I1.h"
#include <I1_static.h>
#include "IDL/Test_I3_factory.h"
#include <gtest/gtest.h>
#include <Nirvana/signal_defs.h>

using namespace CORBA;
using namespace Test;

namespace TestORB {

// The fixture for testing complex local interface.

#ifdef LEGACY_CORBA_CPP
typedef I3_var I3_ref;
#else
typedef I3::_ref_type I3_ref;
#endif

typedef I3_ref (*I3_Factory) ();

class TestORB_I3 :
	public ::testing::TestWithParam <I3_Factory>
{
protected:
	TestORB_I3 ()
	{}

	virtual ~TestORB_I3 ()
	{}

	I3_ref instantiate ()
	{
		return (GetParam ()) ();
	}
};

I3_ref I3_get_dynamic ()
{
	return I3_factory_dynamic->create (MAGIC_CONST);
}

I3_ref I3_get_portable ()
{
	return I3_factory_portable->create (MAGIC_CONST);
}

I3_ref I3_get_static ()
{
#ifdef LEGACY_CORBA_CPP
	return I3::_duplicate (I3_static);
#else
	return I3::_ptr_type (I3_static);
#endif
}

I3_ref I3_get_tied ()
{
	return I3_factory_tied->create (MAGIC_CONST);
}

I3_ref I3_get_tied_derived ()
{
	return I3_tied_derived->create (MAGIC_CONST);
}

INSTANTIATE_TEST_SUITE_P (ServantTypesI3, TestORB_I3, testing::Values (
	I3_get_dynamic,      // 0
	I3_get_portable,     // 1
	I3_get_static,       // 2
	I3_get_tied,         // 3
	I3_get_tied_derived  // 4
));

TEST_P (TestORB_I3, Interface)
{
	I3_ref p = instantiate ();
	test_interface (p);
}

TEST_P (TestORB_I3, Performance)
{
	I3_ref p = instantiate ();
	test_performance (p);
}

TEST_P (TestORB_I3, MultiInherit)
{
	I3_ref p = instantiate ();

	EXPECT_EQ (p->op1 (1), MAGIC_CONST + 1);
	EXPECT_EQ (p->op2 (1), 2 * MAGIC_CONST + 1);
	EXPECT_EQ (p->op3 (1), 3 * MAGIC_CONST + 1);

	{
#ifdef LEGACY_CORBA_CPP
		I1_ptr p1 = p;
		EXPECT_EQ (p1->op1 (1), MAGIC_CONST + 1);
		I3_ptr p3 = I3::_narrow (p1);
		ASSERT_FALSE (is_nil (p3));
		EXPECT_EQ (p3->op3 (1), 3 * MAGIC_CONST + 1);
		release (p3);
#else
		I1::_ref_type p1 = p;
		EXPECT_EQ (p1->op1 (1), MAGIC_CONST + 1);
		I3::_ref_type p3 = I3::_narrow (p1);
		ASSERT_TRUE (p3);
		EXPECT_EQ (p3->op3 (1), 3 * MAGIC_CONST + 1);
#endif
	}

	{
#ifdef LEGACY_CORBA_CPP
		I2_ptr p2 = p;
		EXPECT_EQ (p2->op2 (1), 2 * MAGIC_CONST + 1);
		I3_ptr p3 = I3::_narrow (p2);
		ASSERT_FALSE (is_nil (p3));
		EXPECT_EQ (p3->op3 (1), 3 * MAGIC_CONST + 1);
		release (p3);
#else
		I2::_ref_type p2 = p;
		EXPECT_EQ (p2->op2 (1), 2 * MAGIC_CONST + 1);
		I3::_ref_type p3 = I3::_narrow (p2);
		ASSERT_TRUE (p3);
		EXPECT_EQ (p3->op3 (1), 3 * MAGIC_CONST + 1);
#endif
	}

	{
#ifdef LEGACY_CORBA_CPP
		Object_ptr obj = p;
		ASSERT_FALSE (is_nil (obj));
		I1_ptr p1 = I1::_narrow (obj);
		ASSERT_FALSE (is_nil (p1));
		I2_ptr p2 = I2::_narrow (obj);
		ASSERT_FALSE (is_nil (p2));
		I3_ptr p3 = I3::_narrow (obj);
		ASSERT_FALSE (is_nil (p3));
		ASSERT_TRUE (p->_is_equivalent (p3));
		A1_ptr a1 = A1::_narrow (obj);
		ASSERT_TRUE (a1);
		release (p1);
		release (p2);
		release (p3);
		release (a1);
#else
		Object::_ref_type obj = p;
		ASSERT_TRUE (obj);
		I1::_ref_type p1 = I1::_narrow (obj);
		EXPECT_TRUE (p1);
		I2::_ref_type p2 = I2::_narrow (obj);
		EXPECT_TRUE (p2);
		I3::_ref_type p3 = I3::_narrow (obj);
		EXPECT_TRUE (p3);
		EXPECT_TRUE (p->_is_equivalent (p3));
		A1::_ref_type a1 = A1::_narrow (obj);
		EXPECT_TRUE (a1);
#endif
	}

	{ // Abstract interface
#ifdef LEGACY_CORBA_CPP
		A1_ptr a1 = p;
		AbstractBase_ptr ab = a1;
		ASSERT_TRUE (ab);
		EXPECT_FALSE (ab->_to_value ());
		Object::_ptr_type obj = ab->_to_object ();
		ASSERT_TRUE (obj);
		I3::_ptr_type p3 = I3::_narrow (obj);
		EXPECT_TRUE (p3);
		EXPECT_TRUE (p->_is_equivalent (p3));
		release (obj);
		release (p3);
#else
		A1::_ptr_type a1 = p;
		AbstractBase::_ptr_type ab = a1;
		ASSERT_TRUE (ab);
		EXPECT_FALSE (ab->_to_value ());
		Object::_ref_type obj = ab->_to_object ();
		ASSERT_TRUE (obj);
		I3::_ref_type p3 = I3::_narrow (obj);
		EXPECT_TRUE (p3);
		EXPECT_TRUE (p->_is_equivalent (p3));
#endif
	}
}

TEST_P (TestORB_I3, ObjectOperations)
{
	I3_ref p = instantiate ();
	bool thrown = false;
	try {
		DomainManagersList dml = p->_get_domain_managers ();
	} catch (const NO_IMPLEMENT& ex) {
		thrown = ex.minor () == MAKE_OMG_MINOR (8); // Operation not implemented in local object.
	}
	EXPECT_TRUE (thrown);
	
	thrown = false;
	try {
		p->_get_policy (0);
	} catch (const NO_IMPLEMENT& ex) {
		thrown = ex.minor () == MAKE_OMG_MINOR (8); // Operation not implemented in local object.
	}
	EXPECT_TRUE (thrown);

	IDL::String rep_id = p->_repository_id ();
	EXPECT_EQ (rep_id, "IDL:Test/I3:1.0");
}

}
