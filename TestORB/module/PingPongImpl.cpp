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
	PingImpl (Pong::_ptr_type pong) :
		pong_ (pong),
		count_ (0),
		event_channel_ (g_ORB->create_typed_channel ()),
		cancelled_ (false)
	{
		TypedProxyPushConsumer::_ref_type consumer = event_channel_->for_suppliers ()->
			obtain_typed_push_consumer (_tc_PingResult->id ());
		consumer->connect_push_supplier (nullptr);
		result_consumer_ = PingResult::_narrow (consumer->get_typed_consumer ());

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

class ping_pong_factory :
	public servant_traits <PingPongFactory>::ServantStatic <ping_pong_factory>
{
public:
	static Pong::_ref_type create_pong (uint32_t count)
	{
		return make_reference <PongImpl> (count)->_this ();
	}

	static Ping::_ref_type create_ping (Pong::_ptr_type pong)
	{
		return make_reference <PingImpl> (pong)->_this ();
	}
};

class ping_pong_factory_ping_sysdomain :
	public servant_traits <PingPongFactory>::ServantStatic <ping_pong_factory_ping_sysdomain>
{
public:
	static Pong::_ref_type create_pong (uint32_t count)
	{
		return make_reference <PongImpl> (count)->_this ();
	}

	static Ping::_ref_type create_ping (Pong::_ptr_type pong)
	{
		SysDomain::_ref_type sys_domain = SysDomain::_narrow (g_ORB->resolve_initial_references ("SysDomain"));
		ProtDomain::_ref_type prot_domain = sys_domain->prot_domain ();
		PingPongFactory::_ref_type factory = PingPongFactory::_narrow (prot_domain->bind (StaticId <ping_pong_factory>::static_id_));
		return factory->create_ping (pong);
	}
};

class ping_pong_factory_pong_sysdomain :
	public servant_traits <PingPongFactory>::ServantStatic <ping_pong_factory_pong_sysdomain>
{
public:
	static Pong::_ref_type create_pong (uint32_t count)
	{
		SysDomain::_ref_type sys_domain = SysDomain::_narrow (g_ORB->resolve_initial_references ("SysDomain"));
		ProtDomain::_ref_type prot_domain = sys_domain->prot_domain ();
		PingPongFactory::_ref_type factory = PingPongFactory::_narrow (prot_domain->bind (StaticId <ping_pong_factory>::static_id_));
		return factory->create_pong (count);
	}

	static Ping::_ref_type create_ping (Pong::_ptr_type pong)
	{
		return make_reference <PingImpl> (pong)->_this ();
	}
};

}

NIRVANA_STATIC_EXP (Test, ping_pong_factory)
NIRVANA_STATIC_EXP (Test, ping_pong_factory_ping_sysdomain)
NIRVANA_STATIC_EXP (Test, ping_pong_factory_pong_sysdomain)
