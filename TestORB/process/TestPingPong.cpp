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
#include "TestORB.h"
#include <PingPongFactory.h>

using namespace CORBA;
using namespace Test;

namespace TestORB {

TEST_F (TestORB, PingPong)
{
	static const uint32_t COUNT = 100;

	Pong::_ref_type pong = Nirvana::Static <ping_pong_factory>::ptr ()->create_pong ();
	Ping::_ref_type ping = Nirvana::Static <ping_pong_factory>::ptr ()->create_ping (pong, COUNT);

	auto poller = ping->sendp_wait ();

	uint32_t count = 0;
	try {
		poller->wait (1000, count);
	} catch (const TIMEOUT&) {
		ping->cancel ();
		ADD_FAILURE ();
	}
	EXPECT_EQ (count, COUNT);
}

}
