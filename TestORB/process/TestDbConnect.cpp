/*
* Nirvana test suite.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2025 Igor Popov.
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
#include <gtest/gtest.h>
#include <random>
#include <deque>
#include <Nirvana/File.h>
#include <Nirvana/posix_defs.h>
#include <Nirvana/POSIX.h>
#include "IDL/DbConnect.h"

namespace TestORB
{

class TestDbConnect : public ::testing::TestWithParam<Test::DbConnectFactory::Implementation>
{
public:
	virtual void SetUp ()
	{
		// Code here will be called immediately after the constructor (right
		// before each test).
		auto ns = CosNaming::NamingContextExt::_narrow (CORBA::the_orb->resolve_initial_references ("NameService"));
		ASSERT_TRUE (ns);
		auto tmp_dir = Nirvana::Dir::_narrow (ns->resolve_str ("/var/tmp"));

		// Create temporary file
		const char PATTERN [] = "testXXXXXX.db";
		IDL::String file_name = PATTERN;
		file_ = tmp_dir->mkostemps (file_name, 3, O_DIRECT, 0)->file ();
		url_ = "file:/var/tmp/" + file_name + "?mode=rwc&journal_mode=WAL";
	}

	virtual void TearDown ()
	{
		// Code here will be called immediately after each test (right
		// before the destructor).

		file_->remove ();
	}

	std::string random_string ();

protected:
	std::string url_;
	std::mt19937 rndgen_;

private:
	Nirvana::File::_ref_type file_;
};

INSTANTIATE_TEST_SUITE_P (Implementations, TestDbConnect, testing::Values (
	Test::DbConnectFactory::Implementation::SingleConnection,
	Test::DbConnectFactory::Implementation::ConnectionPool));

std::string TestDbConnect::random_string ()
{
	size_t len = std::uniform_int_distribution <size_t> (0, 1024) (rndgen_);
	std::string s;
	s.reserve (len);
	for (; len; --len) {
		s.push_back ((char)std::uniform_int_distribution <int> (' ', '~') (rndgen_));
	}
	return s;
}

typedef ::Test::AMI_DbConnectPoller Poller;

enum class Operation
{
	Set, Del, Select
};

struct Request
{
	Poller::_ref_type poller;
	int iteration;
	Operation op;

	void complete ();
};

TEST_P (TestDbConnect, Create)
{
	auto obj = ::Test::db_connect_factory->create (GetParam (), SQLite::driver, url_, "", "");
}

TEST_P (TestDbConnect, Random)
{
	auto obj = ::Test::db_connect_factory->create (GetParam (), SQLite::driver, url_, "", "");

	std::deque <Request> active_requests;

	std::bernoulli_distribution dist_write (0.2);
	std::bernoulli_distribution dist_set (0.5);
	std::uniform_int_distribution <int32_t> dist_id (1, 10000);
	int iterations = std::min (std::numeric_limits <int>::max (), 100);
	size_t max_concurrent_requests = 2;

	for (int i = 0; i < iterations; ++i) {
		Operation op;
		int32_t id;
		if (dist_write (rndgen_)) {
			if (dist_set (rndgen_))
				op = Operation::Set;
			else
				op = Operation::Del;
			id = dist_id (rndgen_);
		} else
			op = Operation::Select;

		Request newrq;
		newrq.op = op;
		newrq.iteration = i;
		switch (op) {
			case Operation::Set:
				newrq.poller = obj->sendp_set (id, random_string ());
				ASSERT_EQ (newrq.poller->operation_name (), "set");
				break;
			case Operation::Del:
				newrq.poller = obj->sendp_del (id);
				ASSERT_EQ (newrq.poller->operation_name (), "del");
				break;
			case Operation::Select:
				newrq.poller = obj->sendp_select ();
				ASSERT_EQ (newrq.poller->operation_name (), "select");
				break;
		}

		active_requests.push_back (std::move (newrq));

		for (;;) {
			bool some_finished = false;
			for (auto it = active_requests.begin (); it != active_requests.end ();) {
				if (it->poller->is_ready (0)) {
					Request rq = std::move (*it);
					it = active_requests.erase (it);
					some_finished = true;
					rq.complete ();
				} else
					++it;
			}
			if (active_requests.size () >= max_concurrent_requests) {
				if (!some_finished)
					Nirvana::the_posix->sleep (100 * TimeBase::MILLISECOND);
			} else
				break;
		}
	}

	while (!active_requests.empty ()) {
		Request rq = std::move (active_requests.front ());
		active_requests.pop_front ();
		rq.complete ();
	}

	Nirvana::the_posix->sleep (1 * TimeBase::SECOND);
}

void Request::complete ()
{
	std::string opname = poller->operation_name ();
	switch (op) {
		case Operation::Set:
			ASSERT_EQ (opname, "set");
			poller->set (std::numeric_limits <uint32_t>::max ());
			break;
		case Operation::Del:
			ASSERT_EQ (opname, "del");
			poller->del (std::numeric_limits <uint32_t>::max ());
			break;
		case Operation::Select:
			ASSERT_EQ (opname, "select");
			{
				NDBC::ResultSet::_ref_type rs;
				poller->select (std::numeric_limits <uint32_t>::max (), rs);
				while (rs->next ()) {
					;
				}
			}
			break;
	}
}

}
