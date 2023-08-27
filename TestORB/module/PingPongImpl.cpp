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
#include <CORBA/Server.h>
#include <IDL/PingPong_s.h>
#include <CORBA/CosEventChannelAdmin.h>
#include <PingPongFactory.h>

using namespace CORBA;
using namespace CosEventChannelAdmin;

namespace Test {

class PongImpl : public servant_traits <Pong>::Servant <PongImpl>
{
public:
	void ping ()
	{}
};

class PingImpl : public servant_traits <Ping>::Servant <PingImpl>
{
public:
	PingImpl (Pong::_ptr_type pong, uint32_t count) :
		pong_ (pong),
		count_ (count),
		cur_count_ (0),
		cancelled_ (false)
	{
		event_channel_ = g_ORB->create_event_channel ();
		pull_supplier_ = event_channel_->for_consumers ()->obtain_pull_supplier ();
		pull_supplier_->connect_pull_consumer (nullptr);
		send ();
	}

	~PingImpl ()
	{
		event_channel_->destroy ();
	}

	uint32_t wait ()
	{
		Any any = pull_supplier_->pull ();
		uint32_t ret = 0;
		any >>= ret;
		return ret;
	}

	void cancel ()
	{
		if (!cancelled_) {
			cancelled_ = true;
			finish ();
		}
	}

	void ping ()
	{
		++cur_count_;
		send ();
	}

	void ping_excep (Messaging::ExceptionHolder::_ptr_type eh)
	{
		finish ();
	}

private:
	void send ()
	{
		if (!cancelled_) {
			if (cur_count_ < count_)
				pong_->sendc_ping (_this ());
			else
				finish ();
		}
	}

	void finish ()
	{
		ProxyPushConsumer::_ref_type consumer = event_channel_->for_suppliers ()->obtain_push_consumer ();
		consumer->connect_push_supplier (nullptr);
		Any any;
		any <<= cur_count_;
		consumer->push (any);
		consumer->disconnect_push_consumer ();
	}

private:
	Pong::_ref_type pong_;
	const uint32_t count_;
	uint32_t cur_count_;
	EventChannel::_ref_type event_channel_;
	ProxyPullSupplier::_ref_type pull_supplier_;
	bool cancelled_;
};

class ping_pong_factory :
	public servant_traits <PingPongFactory>::ServantStatic <ping_pong_factory>
{
public:
	static Pong::_ref_type create_pong ()
	{
		return make_reference <PongImpl> ()->_this ();
	}

	static Ping::_ref_type create_ping (Pong::_ptr_type pong, uint32_t count)
	{
		return make_reference <PingImpl> (pong, count)->_this ();
	}
};

}

NIRVANA_STATIC_EXP (Test, ping_pong_factory)
