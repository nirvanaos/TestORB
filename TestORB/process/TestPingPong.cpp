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
#include <CORBA/CORBA.h>
#include <gtest/gtest.h>
#include <PingPongFactory.h>

using namespace CORBA;
using namespace Test;

namespace TestORB {

template <class Factory>
class TestPingPong :
	public ::testing::Test
{
protected:
	TestPingPong ()
	{}

	virtual ~TestPingPong ()
	{}

	static PingPongFactory::_ptr_type factory ()
	{
		return Factory::ptr ();
	}
};

typedef ::testing::Types <Nirvana::Static <ping_pong_factory> // 0
	, Nirvana::Static <ping_pong_factory_ping_sysdomain> // 1
	, Nirvana::Static <ping_pong_factory_pong_sysdomain> // 2
> ServantTypesPingPong;

TYPED_TEST_SUITE (TestPingPong, ServantTypesPingPong);

TYPED_TEST (TestPingPong, PingPong)
{
	static const uint32_t COUNT = 100;

	PingPongFactory::_ptr_type factory = TestPingPong <TypeParam>::factory ();
	Pong::_ref_type pong = factory->create_pong (COUNT);
	Ping::_ref_type ping = factory->create_ping (pong);

	auto channel = ping->completed ();
	auto supplier = channel->obtain_typed_pull_supplier (_tc_PullPingResult->id ());

	supplier->connect_pull_consumer (nullptr);
	auto typed_supplier = PullPingResult::_narrow (supplier->get_typed_supplier ());
	ASSERT_TRUE (typed_supplier);

	auto poller = typed_supplier->sendp_pull_completed ();

	uint32_t count = 0;
	try {
		poller->pull_completed (1000, count);
	} catch (const TIMEOUT&) {
		ping->cancel ();
		ADD_FAILURE () << " Timeout";
	}
	supplier->disconnect_pull_supplier ();
	EXPECT_EQ (count, COUNT);
}

}
