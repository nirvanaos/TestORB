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

/// Test for DB connections and asynchronous calls.
/// Also may be considered as a stress test for core.
class TestDbConnect : public ::testing::TestWithParam<Test::DbConnectFactory::Implementation>
{
public:
	// A number of transactions in each test
	static const int TRANSACTION_COUNT = 200;

	// Average time interval between transactions
	static const TimeBase::TimeT TRANSACTION_INTERVAL = 10 * TimeBase::MILLISECOND;

	// Poll timeout, ms
	static const unsigned POLL_TIMEOUT = 20;

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
		file_name = "file:/var/tmp/" + file_name;
		url_rwc_ = file_name + "?mode=rwc";
		url_ro_ = file_name + "?mode=ro";
	}

	virtual void TearDown ()
	{
		// Code here will be called immediately after each test (right
		// before the destructor).

		file_->remove ();
	}

	std::string random_string ();

protected:
	std::string url_rwc_, url_ro_;
	std::mt19937 rndgen_;

private:
	Nirvana::File::_ref_type file_;
};

INSTANTIATE_TEST_SUITE_P (DbConnectImpl, TestDbConnect, testing::Values (
	Test::DbConnectFactory::Implementation::Single
	,
	Test::DbConnectFactory::Implementation::WriterReader
	,
	Test::DbConnectFactory::Implementation::SingleWriterPoolReader
//	,
//	Test::DbConnectFactory::Implementation::Pool
	,
	Test::DbConnectFactory::Implementation::SingleStateless
	,
	Test::DbConnectFactory::Implementation::WriterReaderStateless
	,
	Test::DbConnectFactory::Implementation::SingleWriterPoolReaderStateless
//	,
//	Test::DbConnectFactory::Implementation::PoolStateless
));

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

	bool complete (uint32_t timeout, bool& exc);
};

TEST_P (TestDbConnect, Create)
{
	auto obj = ::Test::db_connect_factory->create (GetParam (), SQLite::driver, url_rwc_, url_ro_, "", "");
}

TEST_P (TestDbConnect, Random)
{
	auto obj = ::Test::db_connect_factory->create (GetParam (), SQLite::driver, url_rwc_, url_ro_, "", "");

	std::deque <Request> active_requests;

	std::bernoulli_distribution dist_write (0.2);
	std::bernoulli_distribution dist_set (0.5);
	std::uniform_int_distribution <int32_t> dist_id (1, 1000);
	std::uniform_int_distribution <TimeBase::TimeT> dist_delay (0, 2 * TRANSACTION_INTERVAL);

	for (int i = 0; i < TRANSACTION_COUNT; ++i) {
		bool exc = false;
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

		Nirvana::SteadyTime issue_time = Nirvana::the_posix->steady_clock ();
		
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

		for (auto it = active_requests.begin (); it != active_requests.end ();) {
			if (it->poller->is_ready (0)) {
				Request rq = std::move (*it);
				it = active_requests.erase (it);
				EXPECT_TRUE (rq.complete (0, exc));
				if (exc)
					break;
			} else
				++it;
		}

		if (exc)
			break;

		Nirvana::SteadyTime next_time = issue_time + dist_delay (rndgen_);
		Nirvana::SteadyTime cur_time = Nirvana::the_posix->steady_clock ();
		if (next_time > cur_time)
			Nirvana::the_posix->sleep (next_time - cur_time);
	}

	while (!active_requests.empty ()) {
		for (auto it = active_requests.begin (); it != active_requests.end ();) {
			bool exc;
			if (it->complete (POLL_TIMEOUT, exc))
				it = active_requests.erase (it);
			else
				++it;
		}
	}
}

bool Request::complete (uint32_t timeout, bool& exc)
{
	std::string opname = poller->operation_name ();
	try {
		switch (op) {
			case Operation::Set:
				EXPECT_EQ (opname, "set");
				poller->set (timeout);
				break;
			case Operation::Del:
				EXPECT_EQ (opname, "del");
				poller->del (timeout);
				break;
			case Operation::Select:
				EXPECT_EQ (opname, "select");
				poller->select (timeout);
				break;
		}
	} catch (const NDBC::SQLException& ex) {
		ADD_FAILURE () << ex.error ().sqlState ();
		exc = true;
	} catch (const CORBA::TIMEOUT& ex) {
		if (ex.minor () == MAKE_OMG_MINOR (1))
			return false;
		ADD_FAILURE () << ex.what ();
		exc = true;
	} catch (const CORBA::SystemException& ex) {
		ADD_FAILURE () << ex.what ();
		exc = true;
	}
	return true;
}

}
