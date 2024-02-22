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
#include <Nirvana/File.h>
#include <Nirvana/NDBC.h>
#include <fnctl.h>

using namespace CORBA;
using namespace Nirvana;
using namespace NDBC;
using namespace CosNaming;

namespace TestSQLite {

// Test for the SQLite database driver.
class TestSQLite :
	public ::testing::Test
{
protected:
	TestSQLite ()
	{}

	virtual ~TestSQLite ()
	{}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp ()
	{
		// Code here will be called immediately after the constructor (right
		// before each test).
		NamingContextExt::_ref_type ns = NamingContextExt::_narrow (g_ORB->resolve_initial_references ("NameService"));
		ASSERT_TRUE (ns);
		Dir::_ref_type tmp_dir = Dir::_narrow (ns->resolve_str ("/var/tmp"));
		
		// Create temporary file
		const char PATTERN [] = "XXXXXX.tmp";
		IDL::String file_name = PATTERN;
		file_ = tmp_dir->mkostemps (file_name, 4, O_DIRECT)->file ();
		url_ = "sqlite:/var/tmp/" + file_name;
	}

	virtual void TearDown ()
	{
		// Code here will be called immediately after each test (right
		// before the destructor).
		file_->remove ();
	}

	Connection::_ref_type connect () const
	{
		return manager->getConnection (url_, "", "");
	}

	Connection::_ref_type connect_readonly () const
	{
		return manager->getConnection (url_ + "?mode=ro", "", "");
	}

private:
	File::_ref_type file_;
	std::string url_;
};

TEST_F (TestSQLite, Connect)
{
	connect ();
}

}
