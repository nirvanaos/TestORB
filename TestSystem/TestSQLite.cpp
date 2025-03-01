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
#include <Nirvana/posix_defs.h>
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
		NamingContextExt::_ref_type ns = NamingContextExt::_narrow (the_orb->resolve_initial_references ("NameService"));
		ASSERT_TRUE (ns);
		Dir::_ref_type tmp_dir = Dir::_narrow (ns->resolve_str ("/var/tmp"));

		// Create temporary file
		const char PATTERN [] = "testXXXXXX.db";
		IDL::String file_name = PATTERN;
		file_ = tmp_dir->mkostemps (file_name, 3, O_DIRECT, 0)->file ();
		url_ = "file:/var/tmp/" + file_name;
	}

	virtual void TearDown ()
	{
		// Code here will be called immediately after each test (right
		// before the destructor).

		file_->remove ();
	}

	void connect (const char* params, Connection::_ref_type& conn) const
	{
		ASSERT_NOSQLEXCEPTION (conn = SQLite::driver->connect (url_ + params, "", ""));
	}

	void create_test_table (Connection::_ref_type& conn) const;

	std::string random_string ();

	void prepare_insert (Connection::_ptr_type conn, PreparedStatement::_ref_type& stmt);
	void prepare_select (Connection::_ptr_type conn, PreparedStatement::_ref_type& stmt);

	static void clear_statement (Statement::_ptr_type stm);

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
	select->close ();
}

TEST_F (TestSQLite, AutoCommit)
{
	Connection::_ref_type conn;
	ASSERT_NO_FATAL_FAILURE (create_test_table (conn));
	EXPECT_TRUE (conn->getAutoCommit ());

	EXPECT_THROW (conn->commit (), SQLException);
	EXPECT_THROW (conn->rollback (nullptr), SQLException);

	conn->setAutoCommit (false);
	EXPECT_FALSE (conn->getAutoCommit ());

	{
		PreparedStatement::_ref_type insert;
		ASSERT_NO_FATAL_FAILURE (prepare_insert (conn, insert));
		ASSERT_NOSQLEXCEPTION (insert->setString (1, random_string ()));
		ASSERT_NOSQLEXCEPTION (insert->execute ());
		insert->close ();
	}

	conn->commit ();
	EXPECT_FALSE (conn->getAutoCommit ());

	{
		PreparedStatement::_ref_type insert;
		ASSERT_NO_FATAL_FAILURE (prepare_insert (conn, insert));
		ASSERT_NOSQLEXCEPTION (insert->setString (1, random_string ()));
		ASSERT_NOSQLEXCEPTION (insert->execute ());
		insert->close ();
	}

	conn->rollback (nullptr);
	EXPECT_FALSE (conn->getAutoCommit ());

	ASSERT_NOSQLEXCEPTION (conn->rollback (nullptr));

	conn->setAutoCommit (true);
	EXPECT_TRUE (conn->getAutoCommit ());
}

TEST_F (TestSQLite, MultiStat)
{
	Connection::_ref_type conn;
	ASSERT_NO_FATAL_FAILURE (create_test_table (conn));
	{
		PreparedStatement::_ref_type insert;
		ASSERT_NO_FATAL_FAILURE (prepare_insert (conn, insert));
		ASSERT_NOSQLEXCEPTION (insert->setString (1, "Test 1"));
		ASSERT_NOSQLEXCEPTION (insert->execute ());
		ASSERT_NOSQLEXCEPTION (insert->setString (1, "Test 2"));
		ASSERT_NOSQLEXCEPTION (insert->execute ());
		insert->close ();
	}

	Statement::_ref_type stm = conn->createStatement (ResultSet::Type::TYPE_FORWARD_ONLY);
	ResultSet::_ref_type rs;
	ASSERT_NOSQLEXCEPTION (rs = stm->executeQuery (
		"SELECT str FROM test_table WHERE id=1;"
		"SELECT str FROM test_table WHERE id=2"
	));

	ASSERT_TRUE (rs->next ());
	EXPECT_EQ (rs->getString (1), "Test 1");
	ASSERT_TRUE (stm->getMoreResults ());
	rs = stm->getResultSet ();
	ASSERT_TRUE (rs->next ());
	EXPECT_EQ (rs->getString (1), "Test 2");
	rs->close ();
	ASSERT_FALSE (stm->getMoreResults ());
	ASSERT_EQ (stm->getUpdateCount (), -1);
	stm->close ();
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

TEST_F (TestSQLite, ColumnNames)
{
	Connection::_ref_type conn;
	ASSERT_NO_FATAL_FAILURE (create_test_table (conn));
	PreparedStatement::_ref_type select;
	ASSERT_NO_FATAL_FAILURE (prepare_select (conn, select));
	ASSERT_NOSQLEXCEPTION (select->setInt (1, 0));
	ResultSet::_ref_type rs;
	ASSERT_NOSQLEXCEPTION (rs = select->executeQuery ());
	EXPECT_FALSE (rs->next ());
	const auto& columns = rs->getColumnNames ();
	ASSERT_EQ (columns.size (), 1);
	EXPECT_EQ (columns.front (), "str");
}

TEST_F (TestSQLite, FindColumn)
{
	Connection::_ref_type conn;
	ASSERT_NO_FATAL_FAILURE (create_test_table (conn));

	Statement::_ref_type select = conn->createStatement (NDBC::ResultSet::Type::TYPE_FORWARD_ONLY);
	ResultSet::_ref_type rs;
	ASSERT_NOSQLEXCEPTION (rs = select->executeQuery ("SELECT * FROM test_table WHERE id = 0"));
	EXPECT_FALSE (rs->next ());
	EXPECT_EQ (rs->findColumn ("id"), 1);
	EXPECT_EQ (rs->findColumn ("str"), 2);
	EXPECT_THROW (rs->findColumn ("blabla"), NDBC::SQLException);
}

TEST_F (TestSQLite, StatementReuse)
{
	static const unsigned ROW_CNT = 10;

	Connection::_ref_type conn;
	ASSERT_NO_FATAL_FAILURE (create_test_table (conn));
	{
		PreparedStatement::_ref_type insert;
		ASSERT_NO_FATAL_FAILURE (prepare_insert (conn, insert));
		for (unsigned i = 0; i < ROW_CNT; ++i) {
			ASSERT_NOSQLEXCEPTION (insert->setString (1, random_string ()));
			ASSERT_NOSQLEXCEPTION (insert->execute ());
		}
		insert->close ();
	}

	Statement::_ref_type stm = conn->createStatement (ResultSet::Type::TYPE_FORWARD_ONLY);
	
	stm->executeUpdate ("CREATE TABLE test_table2 (str TEXT)");

	clear_statement (stm);

	for (int pass = 0; pass < 2; ++pass) {
		{
			ResultSet::_ref_type rs;
			ASSERT_NOSQLEXCEPTION (rs = stm->executeQuery ("SELECT * FROM test_table"));
			unsigned cnt = 0;
			while (rs->next ())
				++cnt;
			rs->close ();
			ASSERT_EQ (cnt, ROW_CNT);
		}
		clear_statement (stm);
	}
	stm->close ();
}

void TestSQLite::clear_statement (Statement::_ptr_type stm)
{
	while (stm->getMoreResults ())
		;
	stm->getUpdateCount ();
	stm->clearWarnings ();
}

}
