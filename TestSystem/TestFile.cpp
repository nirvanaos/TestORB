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
#include <Nirvana/DirectoryIterator.h>
#include <Nirvana/System.h>
#include <Nirvana/Chrono.h>
#include <Nirvana/posix.h>
#include <random>

using namespace Nirvana;
using namespace CORBA;
using namespace CosNaming;

// Test for filesystem objects.
namespace TestFile {

class TestFile :
	public ::testing::Test
{
protected:
	TestFile ()
	{}

	virtual ~TestFile ()
	{}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp ()
	{
		// Code here will be called immediately after the constructor (right
		// before each test).
		naming_service_ = NamingContextExt::_narrow (the_orb->resolve_initial_references ("NameService"));
		ASSERT_TRUE (naming_service_);
	}

	virtual void TearDown ()
	{
		// Code here will be called immediately after each test (right
		// before the destructor).
		if (temp_file_)
			temp_file_->remove ();
	}

	void create_temp_file (unsigned flags, Access::_ref_type& access);
	void create_temp_file (AccessDirect::_ref_type& access);
	void create_temp_file (AccessBuf::_ref_type& access);

	static void test_write (AccessDirect::_ptr_type file, size_t& file_size, size_t offset, size_t block_size);
	static void test_read (AccessDirect::_ptr_type file, size_t offset, size_t block_size);
	static void test_write (AccessDirect::_ptr_type file);
	static void test_read (AccessDirect::_ptr_type file);

	static void random_write (AccessDirect::_ptr_type file, size_t& file_size, std::mt19937& rndgen,
		size_t max_file_size, size_t max_block_size);
	static void random_read (AccessDirect::_ptr_type file, size_t file_size, std::mt19937& rndgen,
		size_t max_block_size);

	static size_t test_file_size ()
	{
		return 0x1000000;
	}

	static size_t decide_max_block_size (AccessDirect::_ptr_type file)
	{
		return std::max (file->block_size (), (uint32_t)4096);
	}

	static TimeBase::TimeT random_test_max_duration ()
	{
		return 5 * TimeBase::SECOND;
	}

	static unsigned random_test_min_iterations ()
	{
		return 0x1000;
	}

protected:
	NamingContextExt::_ref_type naming_service_;
	File::_ref_type temp_file_;
};

void TestFile::create_temp_file (unsigned flags, Access::_ref_type& access)
{
	// Obtain temporary directory object
	Object::_ref_type obj = naming_service_->resolve_str ("/var/tmp");
	ASSERT_TRUE (obj);
	Dir::_ref_type tmp_dir = Dir::_narrow (obj);
	ASSERT_TRUE (tmp_dir);

	// Create temporary file
	const char PATTERN [] = "XXXXXX.tmp";
	std::string file_name = PATTERN;
	access = tmp_dir->mkostemps (file_name, 4, flags, 0);
	ASSERT_TRUE (access);

	EXPECT_NE (file_name, PATTERN);
	temp_file_ = access->file ();
	EXPECT_EQ (temp_file_->size (), 0);
}

void TestFile::create_temp_file (AccessDirect::_ref_type& access)
{
	Access::_ref_type a;
	ASSERT_NO_FATAL_FAILURE (create_temp_file (O_DIRECT, a));
	access = AccessDirect::_narrow (a->_to_object ());
	ASSERT_TRUE (access);
	EXPECT_EQ (access->size (), 0);
}

void TestFile::create_temp_file (AccessBuf::_ref_type& access)
{
	Access::_ref_type a;
	ASSERT_NO_FATAL_FAILURE (create_temp_file (0, a));
	access = AccessBuf::_downcast (a->_to_value ());
	ASSERT_TRUE (access);
	EXPECT_EQ (access->direct ()->size (), 0);
}

TEST_F (TestFile, Var)
{
	// Obtain "var" directory object
	Object::_ref_type obj = naming_service_->resolve_str ("/var");
	ASSERT_TRUE (obj);
	Dir::_ref_type dir = Dir::_narrow (obj);
	ASSERT_TRUE (dir);

	// Obtain all subdirectories
	BindingIterator::_ref_type it;
	BindingList bindings;
	dir->list (10, bindings, it);
	EXPECT_FALSE (it);

	// "var" must contain "log" and "tmp"
	bool log = false, tmp = false;
	for (const auto& b : bindings) {
		ASSERT_FALSE (b.binding_name ().empty ());
		EXPECT_EQ (b.binding_name ().size (), 1);
		EXPECT_EQ (b.binding_type (), BindingType::ncontext);
		const NameComponent& nc = b.binding_name ().front ();
		EXPECT_TRUE (nc.kind ().empty ());
		if (nc.id () == "log")
			log = true;
		else if (nc.id () == "tmp")
			tmp = true;
	}
	EXPECT_TRUE (log);
	EXPECT_TRUE (tmp);
}

TEST_F (TestFile, TmpIterator)
{
	// Obtain temporary directory object
	Object::_ref_type obj = naming_service_->resolve_str ("/var/tmp");
	ASSERT_TRUE (obj);
	Dir::_ref_type dir = Dir::_narrow (obj);
	ASSERT_TRUE (dir);

	unsigned acc = dir->access ();
	EXPECT_TRUE (acc & F_OK);

	// Iterate
	BindingIterator::_ref_type it;
	BindingList bindings;
	dir->list (0, bindings, it);
	if (it) {
		while (it->next_n (10, bindings)) {
			for (const auto& b : bindings) {
				ASSERT_EQ (b.binding_name ().size (), 1);
				const NameComponent& nc = b.binding_name ().front ();
				EXPECT_FALSE (nc.id ().empty () && nc.kind ().empty ());
			}
		}
	}
}

TEST_F (TestFile, Mnt)
{
	// Iterate all drives in "mnt"
	Object::_ref_type obj = naming_service_->resolve_str ("/mnt");
	ASSERT_TRUE (obj);
	Dir::_ref_type mnt = Dir::_narrow (obj);
	ASSERT_TRUE (mnt);

	BindingIterator::_ref_type it;
	BindingList drives;
	mnt->list (std::numeric_limits <uint32_t>::max (), drives, it);
	EXPECT_FALSE (it);
	for (const auto& b : drives) {
		ASSERT_EQ (b.binding_name ().size (), 1);
		EXPECT_EQ (b.binding_type (), BindingType::ncontext);
		const NameComponent& nc = b.binding_name ().front ();
		EXPECT_FALSE (nc.id ().empty ());
		EXPECT_TRUE (nc.kind ().empty ());
		Dir::_ref_type drive;
		try {
			drive = Dir::_narrow (mnt->resolve (b.binding_name ()));
			ASSERT_TRUE (drive);
		} catch (const CORBA::NO_PERMISSION&) {
		} catch (const CORBA::SystemException& ex) {
			ADD_FAILURE () << ex._name ();
		}
		BindingList dir;
		try {
			drive->list (10, dir, it);
		} catch (const CORBA::NO_PERMISSION&) {
		} catch (const CORBA::INTERNAL& ex) {
			EXPECT_EQ (get_minor_errno (ex.minor ()), EBUSY);
		} catch (const CORBA::SystemException& ex) {
			ADD_FAILURE () << nc.id () << ' ' << ex._name ();
		}
	}
}

TEST_F (TestFile, Direct)
{
	AccessDirect::_ref_type fa;
	create_temp_file (fa);
	ASSERT_TRUE (fa);

	EXPECT_EQ (fa->size (), 0);

	unsigned acc = fa->file ()->access ();
	EXPECT_EQ (acc & (F_OK | R_OK | W_OK), F_OK | R_OK | W_OK);

	// Write
	std::vector <uint8_t> wbuf;
	wbuf.resize (1, 1);
	fa->write (0, wbuf, false);

	EXPECT_EQ (fa->size (), 1);

	// Flush buffer
	fa->flush ();

	// Read
	std::vector <uint8_t> rbuf;
	fa->read (0, 1, rbuf);
	EXPECT_EQ (rbuf, wbuf);

	// Obtain file object
	File::_ref_type file = fa->file ();

	// Close file access
	fa->close ();
	fa = nullptr;

	// Open file for reading
	ASSERT_TRUE (file);
	fa = AccessDirect::_narrow (file->open (O_RDONLY | O_DIRECT, 0)->_to_object ());
	ASSERT_TRUE (fa);
	fa->read (0, 1, rbuf);
	EXPECT_EQ (rbuf, wbuf);

	// Close
	fa->close ();
	fa = nullptr;

	// Open buffered access
	AccessBuf::_ref_type fb = AccessBuf::_downcast (file->open (O_RDONLY, 0)->_to_value ());
	ASSERT_TRUE (fb);

	// Read from buffer
	rbuf.resize (1);
	EXPECT_EQ (fb->read (rbuf.data (), 1), 1);
	EXPECT_EQ (rbuf, wbuf);

	// Close
	fb->close ();
	fb = nullptr;
}

TEST_F (TestFile, Buf)
{
	AccessBuf::_ref_type fa;
	create_temp_file (fa);
	ASSERT_TRUE (fa);

	uint8_t wbuf [16];
	wbuf [0] = 1;
	fa->write (wbuf, 1);
	EXPECT_EQ (fa->position (), 1);
	fa->position (0);
	EXPECT_EQ (fa->position (), 0);
	uint8_t rbuf [16];
	EXPECT_EQ (fa->read (rbuf, 1), 1);
	EXPECT_EQ (rbuf [0], wbuf [0]);

	File::_ref_type file = fa->file ();
	fa->close ();
	fa = nullptr;

	ASSERT_TRUE (file);
	fa = AccessBuf::_downcast (file->open (O_RDONLY, 0)->_to_value ());
	ASSERT_TRUE (fa);
	EXPECT_EQ (fa->direct ()->size (), 1);
	EXPECT_EQ (fa->read (rbuf, 1), 1);
	EXPECT_EQ (rbuf [0], wbuf [0]);
	fa->close ();
	fa = nullptr;
}

TEST_F (TestFile, BufSeqRead)
{
	AccessBuf::_ref_type fa;
	create_temp_file (fa);
	ASSERT_TRUE (fa);

	ASSERT_NO_FATAL_FAILURE (test_write (fa->direct ()));
	size_t cb = test_file_size ();
	for (size_t off = 0; off < cb; off += sizeof (size_t)) {
		size_t buf;
		ASSERT_EQ (fa->read (&buf, sizeof (buf)), sizeof (buf)) << off;
		ASSERT_EQ (buf, off) << off;
	}
}

TEST_F (TestFile, BufSeqGetBufRead)
{
	AccessBuf::_ref_type fa;
	create_temp_file (fa);
	ASSERT_TRUE (fa);

	ASSERT_NO_FATAL_FAILURE (test_write (fa->direct ()));
	size_t cb = test_file_size ();
	for (size_t off = 0; off < cb; off += sizeof (size_t)) {
		size_t cb = sizeof (size_t);
		const size_t* p = (const size_t*)fa->get_buffer_read (cb);
		ASSERT_EQ (cb, sizeof (size_t));
		ASSERT_EQ (*p, off);
		fa->release_buffer (sizeof (size_t));
	}
}

TEST_F (TestFile, Size)
{
	AccessDirect::_ref_type fa;
	create_temp_file (fa);
	ASSERT_TRUE (fa);

	const uint64_t FILE_SIZE = 0x100000;
	const uint32_t BLOCK_SIZE = 0x400;

	// Set size
	fa->size (FILE_SIZE);
	EXPECT_EQ (fa->size (), FILE_SIZE);

	// Read
	for (uint64_t pos = 0; pos < FILE_SIZE; pos += BLOCK_SIZE) {
		std::vector <uint8_t> rbuf;
		fa->read (pos, BLOCK_SIZE, rbuf);
		EXPECT_TRUE (std::all_of (rbuf.begin (), rbuf.end (), [&](uint8_t i) {return i == 0; }));
	}

	// Obtain file object
	File::_ref_type file = fa->file ();

	// Close file access
	fa->close ();
	fa = nullptr;
}

TEST_F (TestFile, DirectoryIterator)
{
	// Obtain directory object
	Object::_ref_type obj = naming_service_->resolve_str ("/var/tmp");
	ASSERT_TRUE (obj);
	Dir::_ref_type dir = Dir::_narrow (obj);
	ASSERT_TRUE (dir);

	DirectoryIterator iter (dir, Dir::SKIP_PERMISSION_DENIED);
	while (const DirEntry* p = iter.readdir ()) {
		EXPECT_FALSE (p->name ().id ().empty () && p->name ().kind ().empty ());
	}
}

void clear_directory (Dir::_ptr_type dir)
{
	ASSERT_TRUE (dir);
	DirectoryIterator iter (dir);
	while (const DirEntry* p = iter.readdir ()) {
		Name name (1, p->name ());
		if (S_IFDIR == (p->st ().mode () & S_IFMT))
			clear_directory (Dir::_narrow (dir->resolve (name)));
		dir->unbind (name);
	}
}

TEST_F (TestFile, Directory)
{
	// Make temporary directory
	Name tmp_dir_name = Nirvana::the_system->to_name ("/tmp/test");
	Dir::_ref_type tmp_dir;
	try {
		tmp_dir = Dir::_narrow (naming_service_->bind_new_context (tmp_dir_name));
	} catch (const NamingContext::AlreadyBound&) {
	}
	if (!tmp_dir) {
		tmp_dir = Dir::_narrow (naming_service_->resolve (tmp_dir_name));
		ASSERT_TRUE (tmp_dir);
		clear_directory (tmp_dir);
	}

	// Create and close temporary file
	std::string tmp_file = "XXXXXX.tmp";
	tmp_dir->mkostemps (tmp_file, 4, 0, 0)->close ();

	// Try to remove directory that is not empty
	bool thrown = false;
	try {
		naming_service_->unbind (tmp_dir_name);
	} catch (const SystemException& ex) {
		EXPECT_TRUE (INTERNAL::_downcast (&ex)) << ex._name ();
		EXPECT_EQ (get_minor_errno (ex.minor ()), ENOTEMPTY);
		thrown = true;
	}
	EXPECT_TRUE (thrown);

	// Delete file
	Name tmp_file_name = Nirvana::the_system->to_name (tmp_file);

	try {
		tmp_dir->unbind (tmp_file_name);
	} catch (const SystemException& ex) {
		ADD_FAILURE () << ex._name () << ' ' << get_minor_errno (ex.minor ());
	}

	// Remove empty directory
	EXPECT_NO_THROW (naming_service_->unbind (tmp_dir_name));

	EXPECT_TRUE (tmp_dir->_non_existent ());
}

TEST_F (TestFile, Locator)
{
	AccessDirect::_ref_type fa;
	create_temp_file (fa);
	ASSERT_TRUE (fa);

	File::_ref_type f = fa->file ();
	ASSERT_TRUE (f);

	DirItemId id = f->id ();

	Object::_ref_type obj = naming_service_->resolve (Name (1));
	ASSERT_TRUE (obj);
	FileSystem::_ref_type file_system = FileSystem::_narrow (obj);
	ASSERT_TRUE (file_system);

	DirItem::_ref_type di = file_system->get_item (id);
	ASSERT_TRUE (di);

	File::_ref_type f1 = File::_narrow (di);
	ASSERT_TRUE (f1);

	EXPECT_TRUE (f->_is_equivalent (f1));
}

void TestFile::test_write (AccessDirect::_ptr_type file, size_t& file_size, size_t offset, size_t block_size)
{
	Bytes buffer (block_size);

	size_t init = offset;
	for (size_t* p = (size_t*)buffer.data (), *end = p + block_size / sizeof (size_t); p != end; ++p) {
		*p = init;
		init += sizeof (size_t);
	}
	file->write (offset, buffer, false);
	size_t write_end = offset + block_size;
	if (file_size < write_end)
		file_size = write_end;
	ASSERT_EQ (file->size (), (FileSize)file_size);
}

void TestFile::test_read (AccessDirect::_ptr_type file, size_t offset, size_t block_size)
{
	Bytes buffer;
	file->read (offset, (uint32_t)block_size, buffer);
	ASSERT_EQ (buffer.size (), block_size);
	size_t init = offset;
	for (const size_t* p = (size_t*)buffer.data (), *end = p + buffer.size () / sizeof (size_t); p != end; ++p) {
		ASSERT_EQ (*p, init);
		init += sizeof (size_t);
	}
}

void TestFile::random_write (AccessDirect::_ptr_type file, size_t& file_size, std::mt19937& rndgen,
	size_t max_file_size, size_t max_block_size)
{
	size_t block_size = std::uniform_int_distribution <size_t> (1, max_block_size / sizeof (size_t)) (rndgen);
	size_t max_offset = std::min (max_file_size / sizeof (size_t) - block_size, file_size / sizeof (size_t));
	size_t offset = std::uniform_int_distribution <size_t> (0, max_offset) (rndgen);
	test_write (file, file_size, offset * sizeof (size_t), block_size * sizeof (size_t));
}

void TestFile::random_read (AccessDirect::_ptr_type file, size_t file_size, std::mt19937& rndgen,
	size_t max_block_size)
{
	ASSERT_NE (file_size, 0);

	size_t block_size = std::uniform_int_distribution <size_t> (1, max_block_size / sizeof (size_t)) (rndgen);
	size_t max_offset = file_size / sizeof (size_t);
	
	size_t offset;
	if (max_offset <= block_size) {
		offset = 0;
		block_size = max_offset;
	} else
		offset = std::uniform_int_distribution <size_t> (0, max_offset - block_size) (rndgen);

	test_read (file, offset * sizeof (size_t), block_size * sizeof (size_t));
}

void TestFile::test_write (AccessDirect::_ptr_type file)
{
	const size_t max_file_size = test_file_size ();
	size_t block_size = file->block_size ();
	size_t file_size = 0;
	while (file_size < max_file_size) {
		size_t cb = max_file_size - file_size;
		if (cb > block_size)
			cb = block_size;
		size_t pos = file_size;
		ASSERT_NO_FATAL_FAILURE (test_write (file, file_size, pos, cb)) << "Position: " << pos;
	}
}

void TestFile::test_read (AccessDirect::_ptr_type file)
{
	const size_t file_size = (size_t)file->size ();
	size_t block_size = file->block_size ();
	for (size_t pos = 0; pos < file_size;) {
		size_t cb = file_size - pos;
		if (cb > block_size)
			cb = block_size;
		ASSERT_NO_FATAL_FAILURE (test_read (file, pos, cb)) << "Position: " << pos;
		pos += cb;
	}
}

TEST_F (TestFile, Sequential)
{
	AccessDirect::_ref_type fa;
	create_temp_file (fa);
	ASSERT_TRUE (fa);

	ASSERT_NO_FATAL_FAILURE (test_write (fa));
	ASSERT_NO_FATAL_FAILURE (test_read (fa));
}

TEST_F (TestFile, RandomRead)
{
	AccessDirect::_ref_type fa;
	create_temp_file (fa);
	ASSERT_TRUE (fa);

	ASSERT_NO_FATAL_FAILURE (test_write (fa));

	std::mt19937 rndgen;
	size_t file_size = (size_t)fa->size ();
	const size_t max_block_size = decide_max_block_size (fa);

	TimeBase::TimeT start_time = Nirvana::the_chrono->steady_clock ();
	TimeBase::TimeT duration = random_test_max_duration ();
	unsigned iterations = random_test_min_iterations ();
	unsigned i = 0;
	TimeBase::TimeT end_time;
	for (;; ++i) {
		ASSERT_NO_FATAL_FAILURE (random_read (fa, file_size, rndgen, max_block_size)) << "Iteration: " << i;
		if (i >= iterations) {
			end_time = Nirvana::the_chrono->steady_clock ();
			if (end_time - start_time >= duration)
				break;
		}
	}
	duration = end_time - start_time;
	std::cout << ((double)i / ((double)(duration) / (double)TimeBase::SECOND)) << " reads per second\n";
}

TEST_F (TestFile, RandomWrite)
{
	AccessDirect::_ref_type fa;
	create_temp_file (fa);
	ASSERT_TRUE (fa);

	const size_t max_block_size = decide_max_block_size (fa);
	const size_t max_file_size = test_file_size ();

	std::mt19937 rndgen;
	size_t file_size = 0;

	TimeBase::TimeT start_time = Nirvana::the_chrono->steady_clock ();
	TimeBase::TimeT duration = random_test_max_duration ();
	unsigned iterations = random_test_min_iterations ();
	unsigned i = 0;
	TimeBase::TimeT end_time;
	for (;; ++i) {
		ASSERT_NO_FATAL_FAILURE (random_write (fa, file_size, rndgen, max_file_size, max_block_size)) << "Iteration: " << i;
		if (i >= iterations) {
			end_time = Nirvana::the_chrono->steady_clock ();
			if (end_time - start_time >= duration)
				break;
		}
	}
	duration = end_time - start_time;

	test_read (fa);

	std::cout << ((double)i / ((double)(duration) / (double)TimeBase::SECOND)) << " writes per second\n";
}

TEST_F (TestFile, Random)
{
	AccessDirect::_ref_type fa;
	create_temp_file (fa);
	ASSERT_TRUE (fa);

	const size_t max_block_size = decide_max_block_size (fa);
	const size_t max_file_size = test_file_size ();

	std::mt19937 rndgen;
	size_t file_size = 0;

	TimeBase::TimeT start_time = Nirvana::the_chrono->steady_clock ();
	TimeBase::TimeT duration = random_test_max_duration ();
	unsigned iterations = random_test_min_iterations ();
	unsigned i = 0;
	TimeBase::TimeT end_time;
	for (;; ++i) {
		if (!std::bernoulli_distribution (((double)file_size / (double)max_file_size) * 0.5) (rndgen)) {
			// Write
			ASSERT_NO_FATAL_FAILURE (random_write (fa, file_size, rndgen, max_file_size, max_block_size)) << "Iteration: " << i;
		} else {
			// Read
			ASSERT_NO_FATAL_FAILURE (random_read (fa, file_size, rndgen, max_block_size)) << "Iteration: " << i;
		}
		if (i >= iterations) {
			end_time = Nirvana::the_chrono->steady_clock ();
			if (end_time - start_time >= duration)
				break;
		}
	}
	duration = end_time - start_time;
	std::cout << ((double)i / ((double)(duration) / (double)TimeBase::SECOND)) << " reads/writes per second\n";
}

TEST_F (TestFile, Stat)
{
	AccessDirect::_ref_type fa;
	create_temp_file (fa);
	ASSERT_TRUE (fa);
	File::_ref_type file = fa->file ();
	ASSERT_TRUE (file);
	FileStat stat;
	file->stat (stat);
	unsigned mode = stat.mode ();
	EXPECT_EQ (mode & S_IFMT, S_IFREG);
}

}
