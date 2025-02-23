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
#include "IDL/DbConnect_s.h"

using namespace CORBA;
using namespace NDBC;

namespace Test {

// NOTE: We can't use virtual classes in stateless servant

template <class Impl>
class DbConnectImpl : public servant_traits <DbConnect>::Servant <Impl>
{
public:
	void create_database () const
	{
		auto conn = static_cast <const Impl&> (*this).get_connection_rw ();
		Statement::_ref_type st = conn->createStatement (ResultSet::Type::TYPE_FORWARD_ONLY);
		st->execute ("CREATE TABLE test(id INTEGER PRIMARY KEY, text TEXT)");
		st->close ();
	}

	void set (int32_t id, const IDL::String& text) const
	{
		auto conn = static_cast <const Impl&> (*this).get_connection_rw ();
		PreparedStatement::_ref_type st = conn->prepareStatement (
			"INSERT OR REPLACE INTO test VALUES(?,?)",
			ResultSet::Type::TYPE_FORWARD_ONLY, 0);
		st->setInt (1, id);
		st->setString (2, text);
		st->executeUpdate ();
		st->close ();
	}

	void del (int32_t id) const
	{
		auto conn = static_cast <const Impl&> (*this).get_connection_rw ();
		PreparedStatement::_ref_type st = conn->prepareStatement (
			"DELETE FROM test WHERE id=?",
			ResultSet::Type::TYPE_FORWARD_ONLY, 0);
		st->setInt (1, id);
		st->executeUpdate ();
		st->close ();
	}

	ResultSet::_ref_type select () const
	{
		auto conn = static_cast <const Impl&> (*this).get_connection_ro ();
		Statement::_ref_type st = conn->createStatement (ResultSet::Type::TYPE_FORWARD_ONLY);
		return st->executeQuery ("SELECT * FROM test");
	}

};

class DbConnectSingle : public DbConnectImpl <DbConnectSingle>
{
	static const TimeBase::TimeT TIMEOUT = TimeBase::SECOND * 10;

public:
	DbConnectSingle (Driver::_ref_type driver, const IDL::String& url_rwc, const IDL::String& url_ro,
		const IDL::String& user, const IDL::String& password) :
		connection_rw_ (driver->connect (url_rwc, user, password)),
		connection_ro_ (driver->connect (url_ro, user, password))
	{
		connection_rw_->setTimeout (TIMEOUT);
		connection_ro_->setTimeout (TIMEOUT);
		create_database ();
	}

	~DbConnectSingle ()
	{}

	Connection::_ptr_type get_connection_rw () const
	{
		return connection_rw_;
	}

	Connection::_ptr_type get_connection_ro () const
	{
		return connection_ro_;
	}

private:
	const Connection::_ref_type connection_rw_, connection_ro_;
};

class DbConnectPool : public DbConnectImpl <DbConnectPool>
{
	static const TimeBase::TimeT TIMEOUT = TimeBase::SECOND * 10;

public:
	DbConnectPool (Driver::_ref_type driver, const IDL::String& url_rwc, const IDL::String& url_ro,
		const IDL::String& user, const IDL::String& password) :
		connection_rw_ (
//			driver->connect (url_rwc, user, password)
			the_manager->createConnectionPool (driver, url_rwc, user, password, 0)->getConnection ()
		),
		pool_ro_ (the_manager->createConnectionPool (driver, url_ro, user, password, 0))
	{
		connection_rw_->setTimeout (TIMEOUT);
		create_database ();
	}

	~DbConnectPool ()
	{}

	class PoolableConnection : public Connection::_ref_type
	{
	public:
		PoolableConnection(Connection::_ref_type&& conn) :
			Connection::_ref_type (std::move (conn))
		{}

		~PoolableConnection ()
		{
			p_->close ();
		}
	};

	Connection::_ptr_type get_connection_rw () const
	{
		return connection_rw_;
	}

	PoolableConnection get_connection_ro () const
	{
		return pool_ro_->getConnection ();
	}

private:
	const Connection::_ref_type connection_rw_;
	const ConnectionPool::_ref_type pool_ro_;
};

class Static_db_connect_factory :
	public servant_traits <DbConnectFactory>::ServantStatic <Static_db_connect_factory>
{
public:
	DbConnect::_ref_type create (Implementation impl, Driver::_ptr_type driver,
		const IDL::String& url_rwc, const IDL::String& url_ro,
		const IDL::String& user, const IDL::String& password)
	{
		switch (impl) {
			default:
			case Implementation::SingleConnection:
				return make_reference <DbConnectSingle> (driver, std::ref (url_rwc), std::ref (url_ro),
					std::ref (user), std::ref (password))->_this ();
			case Implementation::ConnectionPool:
				return make_reference <DbConnectPool> (driver, std::ref (url_rwc), std::ref (url_ro),
					std::ref (user), std::ref (password))->_this ();
			case Implementation::SingleConnectionStateless:
				return make_stateless <DbConnectSingle> (driver, std::ref (url_rwc), std::ref (url_ro),
					std::ref (user), std::ref (password))->_this ();
			case Implementation::ConnectionPoolStateless:
				return make_stateless <DbConnectPool> (driver, std::ref (url_rwc), std::ref (url_ro),
					std::ref (user), std::ref (password))->_this ();
		}
	}
};

}

NIRVANA_EXPORT_OBJECT (_exp_Test_db_connect_factory, Test::Static_db_connect_factory)
