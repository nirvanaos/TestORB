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
#include <random>

using namespace CORBA;
using namespace Nirvana;
using namespace NDBC;
using namespace CosNaming;

#define ASSERT_NOSQLEXCEPTION(...) try {\
	__VA_ARGS__;\
} catch (const SQLException& ex) {\
	GTEST_FAIL () << ex.error ().sqlState ();\
}

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
		const char PATTERN [] = "testXXXXXX.db";
		IDL::String file_name = PATTERN;
		file_ = tmp_dir->mkostemps (file_name, 3, O_DIRECT)->file ();
		url_ = "sqlite:/var/tmp/" + file_name;
	}

	virtual void TearDown ()
	{
		// Code here will be called immediately after each test (right
		// before the destructor).
		
		file_->remove ();
	}

	void connect (const char* params, Connection::_ref_type& conn) const
	{
		ASSERT_NOSQLEXCEPTION (conn = manager->getConnection (url_ + params, "", ""));
	}

	void create_test_table (Connection::_ref_type& conn) const;

	std::string random_string ();

	void prepare_insert (Connection::_ptr_type conn, PreparedStatement::_ref_type& stmt);
	void prepare_select (Connection::_ptr_type conn, PreparedStatement::_ref_type& stmt);

private:
	File::_ref_type file_;
	std::string url_;
	std::mt19937 rndgen_;
};

void TestSQLite::create_test_table (Connection::_ref_type& conn) const
{
	ASSERT_NO_FATAL_FAILURE (connect ("?mode=rwc", conn));
	Statement::_ref_type stmt;
	ASSERT_NOSQLEXCEPTION (stmt = conn->createStatement (ResultSet::Type::TYPE_FORWARD_ONLY));
	ASSERT_NOSQLEXCEPTION (stmt->executeUpdate ("CREATE TABLE test_table (id INTEGER PRIMARY KEY AUTOINCREMENT, str TEXT)"));
}

std::string TestSQLite::random_string ()
{
	size_t len = std::uniform_int_distribution <size_t> (0, 1024) (rndgen_);
	std::string s;
	s.reserve (len);
	for (; len; --len) {
		s.push_back ((char)std::uniform_int_distribution <int> (' ', '~') (rndgen_));
	}
	return s;
}

void TestSQLite::prepare_insert (Connection::_ptr_type conn, PreparedStatement::_ref_type& stmt)
{
	ASSERT_NOSQLEXCEPTION (stmt = conn->prepareStatement (
		"INSERT INTO test_table (str) VALUES (?) RETURNING id",
		ResultSet::Type::TYPE_FORWARD_ONLY,
		PreparedStatement::PREPARE_PERSISTENT));
}

void TestSQLite::prepare_select (Connection::_ptr_type conn, PreparedStatement::_ref_type& stmt)
{
	ASSERT_NOSQLEXCEPTION (stmt = conn->prepareStatement (
		"SELECT str FROM test_table WHERE id=?",
		ResultSet::Type::TYPE_FORWARD_ONLY,
		PreparedStatement::PREPARE_PERSISTENT));
}

TEST_F (TestSQLite, Connect)
{
	Connection::_ref_type conn;
	ASSERT_NO_FATAL_FAILURE (connect ("?mode=rwc", conn));
}

TEST_F (TestSQLite, CreateTable)
{
	Connection::_ref_type conn;
	ASSERT_NO_FATAL_FAILURE (create_test_table (conn));
}

struct TestTableRow
{
	int32_t id;
	std::string str;
};

typedef std::vector <TestTableRow> TestTableRows;

TEST_F (TestSQLite, Prepared)
{
	Connection::_ref_type conn;
	ASSERT_NO_FATAL_FAILURE (create_test_table (conn));

	TestTableRows rows;

	{
		PreparedStatement::_ref_type insert;
		ASSERT_NO_FATAL_FAILURE (prepare_insert (conn, insert));

		const unsigned ROW_CNT = 100;

		for (unsigned i = 0; i < ROW_CNT; ++i) {
			TestTableRow row;
			row.str = random_string ();
			ASSERT_NOSQLEXCEPTION (insert->setString (1, row.str));
			ResultSet::_ref_type rs;
			ASSERT_NOSQLEXCEPTION (rs = insert->executeQuery ());
			ASSERT_TRUE (rs->next ());
			ASSERT_NOSQLEXCEPTION (row.id = rs->getInt (1));
			rows.push_back (std::move (row));
		}

		insert->close ();
	}

	PreparedStatement::_ref_type select;
	ASSERT_NO_FATAL_FAILURE (prepare_select (conn, select));

	for (const auto& row : rows) {
		ASSERT_NOSQLEXCEPTION (select->setInt (1, row.id));
		ResultSet::_ref_type rs;
		ASSERT_NOSQLEXCEPTION (rs = select->executeQuery ());
		ASSERT_TRUE (rs->next ());
		std::string str;
		ASSERT_NOSQLEXCEPTION (str = rs->getString (1));
		EXPECT_EQ (row.str, str);
	}
}

TEST_F (TestSQLite, GetParent)
{
	Connection::_ref_type conn;
	ASSERT_NO_FATAL_FAILURE (create_test_table (conn));

	PreparedStatement::_ref_type insert;
	ASSERT_NO_FATAL_FAILURE (prepare_insert (conn, insert));
	ASSERT_NOSQLEXCEPTION (insert->setString (1, random_string ()));
	ResultSet::_ref_type rs;
	ASSERT_NOSQLEXCEPTION (rs = insert->executeQuery ());

	StatementBase::_ref_type stmt = rs->getStatement ();
	ASSERT_TRUE (stmt);
	EXPECT_TRUE (stmt->_is_equivalent (insert));

	Connection::_ref_type conn1 = stmt->getConnection ();
	ASSERT_TRUE (conn1);
	EXPECT_TRUE (conn1->_is_equivalent (conn));
}

}
