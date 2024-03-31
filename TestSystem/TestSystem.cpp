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
#include <Nirvana/System.h>
#include <Nirvana/POSIX.h>
#include <Nirvana/File.h>
#include <Nirvana/filesystem.h>
#include <signal.h>

using namespace Nirvana;

namespace TestSystem {

// Test for the Nirvana system functionality.
class TestSystem :
	public ::testing::Test
{
protected:
	TestSystem ()
	{}

	virtual ~TestSystem ()
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

	static void writemem (void* p);
};

TEST_F (TestSystem, CreateHeap)
{
	static const size_t GRANULARITY = 128;
	static const size_t BLOCK_SIZE = GRANULARITY * 128;
	static const size_t COUNT = 1024 * 1024 * 4 / 16 * GRANULARITY / BLOCK_SIZE;
	void* blocks [COUNT];
	Memory::_ref_type heap = Nirvana::the_memory->create_heap (GRANULARITY);
	EXPECT_EQ (GRANULARITY, heap->query (0, Memory::QueryParam::ALLOCATION_UNIT));

	for (int i = 0; i < COUNT; ++i) {
		size_t cb = BLOCK_SIZE;
		blocks [i] = heap->allocate (nullptr, cb, 0);
		ASSERT_TRUE (blocks [i]);
		UIntPtr au = heap->query (blocks [i], Memory::QueryParam::ALLOCATION_UNIT);
		ASSERT_EQ (GRANULARITY, au);
	}

	for (int i = COUNT - 1; i >= 0; --i) {
		heap->release (blocks [i], BLOCK_SIZE);
	}
}

TEST_F (TestSystem, AccessViolation)
{
	Memory::_ref_type heap = Nirvana::the_memory->create_heap (0);
	size_t cb = (size_t)heap->query (nullptr, Memory::QueryParam::PROTECTION_UNIT);
	if (!cb)
		return;

	void* p = heap->allocate (0, cb, Memory::RESERVED);
	bool OK = false;
	int minor = 0;
	try {
		writemem (p);
	} catch (const CORBA::SystemException& ex) {
		EXPECT_TRUE (CORBA::ACCESS_VIOLATION::_downcast (&ex)) << ex.what ();
		OK = true;
		minor = ex.minor ();
	}
	heap->release (p, cb);
	EXPECT_TRUE (OK);
	EXPECT_EQ (SEGV_MAPERR, minor);

	p = heap->allocate (0, cb, 0);
	void* p1 = heap->copy (nullptr, p, cb, Memory::READ_ONLY);
	OK = false;
	try {
		writemem (p1);
	} catch (const CORBA::SystemException& ex) {
		EXPECT_TRUE (CORBA::ACCESS_VIOLATION::_downcast (&ex)) << ex.what ();;
		OK = true;
		minor = ex.minor ();
	}
	heap->release (p, cb);
	heap->release (p1, cb);
	EXPECT_TRUE (OK);
	EXPECT_EQ (SEGV_ACCERR, minor);
}

#pragma optimize("", off)
void TestSystem::writemem (void* p)
{
	if (!p)
		throw CORBA::BAD_PARAM ();
	*(int*)p = 1;
}

TEST_F (TestSystem, Yield)
{
	EXPECT_FALSE (Nirvana::the_system->yield ());
}

TEST_F (TestSystem, CurDir)
{
	// Get current working directory name
	CosNaming::Name cur_dir = Nirvana::the_posix->get_current_dir_name ();
	EXPECT_TRUE (is_absolute (cur_dir));

	// Get reference to NameService
	CosNaming::NamingContext::_ref_type ns = CosNaming::NamingContext::_narrow (
		CORBA::the_orb->resolve_initial_references ("NameService"));
	ASSERT_TRUE (ns);

	// Obtain reference to current directory object
	Dir::_ref_type dir = Dir::_narrow (ns->resolve (cur_dir));
	ASSERT_TRUE (dir);

	// Make subdirectory
	CosNaming::Name subdir;
	subdir.emplace_back ("test", "tmp");
	try {
		dir->bind_new_context (subdir);
	} catch (const CosNaming::NamingContext::AlreadyBound&)
	{} // Ignore if exists

	// Change current to subdirectory
	Nirvana::the_posix->chdir ("test.tmp");

	CosNaming::Name cur_dir1 = Nirvana::the_posix->get_current_dir_name ();
	EXPECT_EQ (cur_dir1.size (), cur_dir.size () + 1);

	// Change current back
	Nirvana::the_posix->chdir ("..");

	cur_dir1 = Nirvana::the_posix->get_current_dir_name ();
	EXPECT_EQ (cur_dir1, cur_dir);

	// Remove subdirectory
	dir->unbind (subdir);
}

TEST_F (TestSystem, Sleep)
{
	static const TimeBase::TimeT SLEEP_TIME = TimeBase::SECOND * 1;
	TimeBase::TimeT t0 = Nirvana::the_system->steady_clock ();
	Nirvana::the_posix->sleep (SLEEP_TIME);
	int64_t delay = Nirvana::the_system->steady_clock () - t0 - SLEEP_TIME;
	EXPECT_GE (delay, -1 * (int64_t)TimeBase::MILLISECOND) << delay;
	EXPECT_LE (delay, 20 * (int64_t)TimeBase::MILLISECOND) << delay;
}

TEST_F (TestSystem, ContextType)
{
	EXPECT_EQ (Nirvana::the_system->context_type (), System::ContextType::PROCESS);
}

}
