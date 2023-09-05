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
#include <Nirvana/Domains.h>

using namespace CORBA;
using namespace CosTypedEventChannelAdmin;
using namespace Nirvana;

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
		event_channel_ (g_ORB->create_typed_channel ()),
		cancelled_ (false)
	{
		send ();
	}

	~PingImpl ()
	{
		event_channel_->destroy ();
	}

	CosTypedEventChannelAdmin::TypedConsumerAdmin::_ref_type completed () const
	{
		return event_channel_->for_consumers ();
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
		TypedProxyPushConsumer::_ref_type consumer = event_channel_->for_suppliers ()->
			obtain_typed_push_consumer (_tc_PingResult->id ());
		consumer->connect_push_supplier (nullptr);
		PingResult::_ref_type typed_consumer = PingResult::_narrow (consumer->get_typed_consumer ());
		typed_consumer->completed (cur_count_);
		consumer->disconnect_push_consumer ();
	}

private:
	Pong::_ref_type pong_;
	const uint32_t count_;
	uint32_t cur_count_;
	TypedEventChannel::_ref_type event_channel_;
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

class ping_pong_factory_ping_sysdomain :
	public servant_traits <PingPongFactory>::ServantStatic <ping_pong_factory_ping_sysdomain>
{
public:
	static Pong::_ref_type create_pong ()
	{
		return make_reference <PongImpl> ()->_this ();
	}

	static Ping::_ref_type create_ping (Pong::_ptr_type pong, uint32_t count)
	{
		SysDomain::_ref_type sys_domain = SysDomain::_narrow (g_ORB->resolve_initial_references ("SysDomain"));
		ProtDomain::_ref_type prot_domain = sys_domain->prot_domain ();
		PingPongFactory::_ref_type factory = PingPongFactory::_narrow (prot_domain->bind (StaticId <ping_pong_factory>::static_id_));
		return factory->create_ping (pong, count);
	}
};

class ping_pong_factory_pong_sysdomain :
	public servant_traits <PingPongFactory>::ServantStatic <ping_pong_factory_pong_sysdomain>
{
public:
	static Pong::_ref_type create_pong ()
	{
		SysDomain::_ref_type sys_domain = SysDomain::_narrow (g_ORB->resolve_initial_references ("SysDomain"));
		ProtDomain::_ref_type prot_domain = sys_domain->prot_domain ();
		PingPongFactory::_ref_type factory = PingPongFactory::_narrow (prot_domain->bind (StaticId <ping_pong_factory>::static_id_));
		return factory->create_pong ();
	}

	static Ping::_ref_type create_ping (Pong::_ptr_type pong, uint32_t count)
	{
		return make_reference <PingImpl> (pong, count)->_this ();
	}
};

}

NIRVANA_STATIC_EXP (Test, ping_pong_factory)
NIRVANA_STATIC_EXP (Test, ping_pong_factory_ping_sysdomain)
NIRVANA_STATIC_EXP (Test, ping_pong_factory_pong_sysdomain)
