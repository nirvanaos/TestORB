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
#include <Nirvana/Nirvana.h>
#include <gtest/gtest.h>
#include <Nirvana/Shell.h>

using namespace Nirvana;

namespace TestShell {

// Test for the Nirvana process API.
class TestShell :
	public ::testing::Test
{
protected:
	TestShell ()
	{}

	virtual ~TestShell ()
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

TEST_F (TestShell, Spawn)
{
	SpawnFiles files;
	the_shell->get_spawn_files (files);
	Process::_ref_type process = the_shell->spawn (StringSeq { "ChildProcess.nex" }, files);
	process->wait (std::numeric_limits <TimeBase::TimeT>::max ());
	int32_t ret;
	process->get_exit_code (ret);
	EXPECT_EQ (ret, 0);
}
/*
TEST_F (TestShell, Abort)
{
	SpawnFiles files;
	the_shell->get_spawn_files (files);
	int ret = the_shell->spawn (StringSeq { "ChildProcess.nex", "abort"}, files);
	EXPECT_EQ (ret, -3);
}
*/
}
