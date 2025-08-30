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
#include "idl/PingPong_s.h"
#include <CORBA/CosEventChannelAdmin.h>
#include <Nirvana/Domains.h>

using namespace CORBA;
using namespace CosTypedEventChannelAdmin;
using namespace Nirvana;

namespace Test {

class PongImpl : public servant_traits <Pong>::Servant <PongImpl>
{
public:
	PongImpl (uint32_t count) :
		count_ (count)
	{}

	void ping ()
	{
		if (count_)
			--count_;
		else
			throw ImTired ();
	}

private:
	uint32_t count_;
};

class PingImpl : public servant_traits <Ping>::Servant <PingImpl>
{
public:
	PingImpl () :
		count_ (0),
		event_channel_ (the_orb->create_typed_channel ()),
		cancelled_ (false)
	{
		TypedProxyPushConsumer::_ref_type consumer = event_channel_->for_suppliers ()->
			obtain_typed_push_consumer (_tc_PingResult->id ());
		consumer->connect_push_supplier (nullptr);
		result_consumer_ = PingResult::_narrow (consumer->get_typed_consumer ());
	}

	~PingImpl ()
	{
		event_channel_->destroy ();
	}

	void start (Pong::_ptr_type pong)
	{
		if (pong_)
			throw CORBA::BAD_INV_ORDER ();
		pong_ = pong;
		count_ = 0;
		send ();
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
		++count_;
		send ();
	}

	void ping_excep (Messaging::ExceptionHolder::_ptr_type eh)
	{
		bool ok = false;
		if (eh) {
			// On an error condition, eh may be nil
			try {
				eh->raise_exception ();
			} catch (const ImTired&) {
				ok = true;
			} catch (...) {
			}
		}

		if (!ok)
			count_ = std::numeric_limits <uint32_t>::max (); // Signalize that not all is OK

		finish ();
	}

	uint32_t count () const noexcept
	{
		return count_;
	}

private:
	void send ()
	{
		if (!cancelled_)
			pong_->sendc_ping (_this ());
	}

	void finish ()
	{
		result_consumer_->completed (count_);
	}

private:
	Pong::_ref_type pong_;
	uint32_t count_;
	TypedEventChannel::_ref_type event_channel_;
	PingResult::_ref_type result_consumer_;
	bool cancelled_;
};

class Static_ping_pong_factory :
	public servant_traits <PingPongFactory>::ServantStatic <Static_ping_pong_factory>
{
public:
	static Pong::_ref_type create_pong (uint32_t count)
	{
		return make_reference <PongImpl> (count)->_this ();
	}

	static Ping::_ref_type create_ping ()
	{
		return make_reference <PingImpl> ()->_this ();
	}
};

class Static_ping_pong_factory_ping_sysdomain :
	public servant_traits <PingPongFactory>::ServantStatic <Static_ping_pong_factory_ping_sysdomain>
{
public:
	static Pong::_ref_type create_pong (uint32_t count)
	{
		return make_reference <PongImpl> (count)->_this ();
	}

	static Ping::_ref_type create_ping ()
	{
		SysDomain::_ref_type sys_domain = SysDomain::_narrow (the_orb->resolve_initial_references ("SysDomain"));
		ProtDomain::_ref_type prot_domain = sys_domain->prot_domain ();
		PingPongFactory::_ref_type factory = PingPongFactory::_narrow (prot_domain->bind (Internal::StaticId <Static_ping_pong_factory>::id));
		return factory->create_ping ();
	}
};

class Static_ping_pong_factory_pong_sysdomain :
	public servant_traits <PingPongFactory>::ServantStatic <Static_ping_pong_factory_pong_sysdomain>
{
public:
	static Pong::_ref_type create_pong (uint32_t count)
	{
		SysDomain::_ref_type sys_domain = SysDomain::_narrow (the_orb->resolve_initial_references ("SysDomain"));
		ProtDomain::_ref_type prot_domain = sys_domain->prot_domain ();
		PingPongFactory::_ref_type factory = PingPongFactory::_narrow (prot_domain->bind (Internal::StaticId <Static_ping_pong_factory>::id));
		return factory->create_pong (count);
	}

	static Ping::_ref_type create_ping ()
	{
		return make_reference <PingImpl> ()->_this ();
	}
};

}

NIRVANA_EXPORT_OBJECT (_exp_Test_ping_pong_factory, Test::Static_ping_pong_factory)
NIRVANA_EXPORT_OBJECT (_exp_Test_ping_pong_factory_ping_sysdomain, Test::Static_ping_pong_factory_ping_sysdomain)
NIRVANA_EXPORT_OBJECT (_exp_Test_ping_pong_factory_pong_sysdomain, Test::Static_ping_pong_factory_pong_sysdomain)
