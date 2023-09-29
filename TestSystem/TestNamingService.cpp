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
#include <Nirvana/filesystem.h>

using namespace CORBA;
using namespace CosNaming;

namespace TestNamingService {

class TestNamingService :
	public ::testing::Test
{
protected:
	TestNamingService ()
	{}

	virtual ~TestNamingService ()
	{}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp ()
	{
		// Code here will be called immediately after the constructor (right
		// before each test).
		service_ = NamingContextExt::_narrow (g_ORB->resolve_initial_references ("NameService"));
		ASSERT_TRUE (service_);
	}

	virtual void TearDown ()
	{
		// Code here will be called immediately after each test (right
		// before the destructor).
	}

protected:
	NamingContextExt::_ref_type service_;
};

TEST_F (TestNamingService, Root)
{
	BindingIterator::_ref_type it;
	BindingList bindings;
	service_->list (2, bindings, it);
	EXPECT_FALSE (it);
	ASSERT_EQ (1, bindings.size ());
	EXPECT_EQ (BindingType::ncontext, bindings.front ().binding_type ());
	ASSERT_EQ (1, bindings.front ().binding_name ().size ());
	const NameComponent& nc = bindings.front ().binding_name ().front ();
	EXPECT_TRUE (nc.id ().empty ());
	EXPECT_TRUE (nc.kind ().empty ());
}

TEST_F (TestNamingService, Orphans)
{
	Name name_a{ NameComponent ("A", "") };
	Name name_b{ NameComponent ("B", "") };
	Name name_c{ NameComponent ("C", "") };
	NamingContext::_ref_type a = service_->bind_new_context (name_a);
	NamingContext::_ref_type b = a->bind_new_context (name_b);
	NamingContext::_ref_type c = b->bind_new_context (name_c);
	EXPECT_NO_THROW (service_->unbind (name_a));
	service_->bind_context (name_a, a);

	// Create cyclic
	c->bind_context (name_a, a);

	// Prevent cyclic orphans
	bool thrown = false;
	try {
		service_->unbind (name_a);
	} catch (const NamingContext::CannotProceed& ex) {
		thrown = true;
		EXPECT_TRUE (service_->_is_equivalent (ex.cxt ()));
		EXPECT_EQ (ex.rest_of_name (), name_a);
	}
	EXPECT_TRUE (thrown);

	// Break cycle
	c->unbind (name_a);

	// Now OK
	EXPECT_NO_THROW (service_->unbind (name_a));
}

}
