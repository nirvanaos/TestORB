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
#include <Nirvana/Domains.h>
#include <Nirvana/platform.h>

using namespace Nirvana;

namespace TestDomains {

// Test for the Nirvana system functionality.
class TestDomains :
	public ::testing::Test
{
protected:
	TestDomains ()
	{}

	virtual ~TestDomains ()
	{}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp ()
	{
		// Code here will be called immediately after the constructor (right
		// before each test).
	}

	virtual void TearDown ()
	{
		// Code here will be called immediately after each test (right
		// before the destructor).
	}
};

TEST_F (TestDomains, CreateDomain)
{
	SysDomain::_ref_type sys_domain = SysDomain::_narrow (CORBA::g_ORB->resolve_initial_references ("SysDomain"));
	IDL::Sequence <uint16_t> platforms = sys_domain->supported_platforms ();
	SysManager::_ref_type sys_manager = sys_domain->provide_manager ();
	for (auto pl : platforms) {
		try {
			ProtDomain::_ref_type prot_domain = sys_manager->create_prot_domain (pl);
			prot_domain->user ();
			prot_domain->shutdown (0);
		} catch (const std::exception& ex) {
			ADD_FAILURE () << ex.what ();
		}
	}
}

}
