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

class DbConnectImpl : public servant_traits <DbConnect>::Servant <DbConnectImpl>
{
public:
	void create_database () const;

	void set (int32_t id, const IDL::String& text) const
	{
		Connection::_ref_type conn = get_connection ();
		PreparedStatement::_ref_type st = conn->prepareStatement (
			"INSERT OR REPLACE INTO test VALUES(?,?)",
			ResultSet::Type::TYPE_FORWARD_ONLY, 0);
		st->setInt (1, id);
		st->setString (2, text);
		st->executeUpdate ();
	}

	void del (int32_t id) const
	{
		Connection::_ref_type conn = get_connection ();
		PreparedStatement::_ref_type st = conn->prepareStatement (
			"DELETE FROM test WHERE id=?",
			ResultSet::Type::TYPE_FORWARD_ONLY, 0);
		st->setInt (1, id);
		st->executeUpdate ();
	}

	ResultSet::_ref_type select () const
	{
		Connection::_ref_type conn = get_connection ();
		Statement::_ref_type st = conn->createStatement (ResultSet::Type::TYPE_FORWARD_ONLY);
		return st->executeQuery ("SELECT * FROM test");
	}

protected:
	virtual Connection::_ref_type get_connection () const = 0;
};

void DbConnectImpl::create_database () const
{
	Connection::_ref_type conn = get_connection ();
	Statement::_ref_type st = conn->createStatement (ResultSet::Type::TYPE_FORWARD_ONLY);
	st->execute ("CREATE TABLE test(id INTEGER PRIMARY KEY, text TEXT)");
}

class DbConnectSingle : public DbConnectImpl
{
	static const TimeBase::TimeT TIMEOUT = TimeBase::SECOND * 10;

public:
	DbConnectSingle (Driver::_ref_type driver, const IDL::String& url,
		const IDL::String& user, const IDL::String& password) :
		connection_ (driver->connect (url, user, password))
	{
		connection_->setTimeout (TIMEOUT);
		create_database ();
	}

protected:
	Connection::_ref_type get_connection () const override
	{
		return connection_;
	}

private:
	Connection::_ref_type connection_;
};

class DbConnectPool : public DbConnectImpl
{
public:
	DbConnectPool (Driver::_ref_type driver, const IDL::String& url,
		const IDL::String& user, const IDL::String& password) :
		pool_ (the_manager->createConnectionPool (driver, url, user, password, 0))
	{
		create_database ();
	}

protected:
	Connection::_ref_type get_connection () const override
	{
		return pool_->getConnection ();
	}

private:
	ConnectionPool::_ref_type pool_;
};

class Static_db_connect_factory :
	public servant_traits <DbConnectFactory>::ServantStatic <Static_db_connect_factory>
{
public:
	DbConnect::_ref_type create (Implementation impl, Driver::_ptr_type driver,
		const IDL::String& url, const IDL::String& user, const IDL::String& password)
	{
		switch (impl) {
			default:
			case Implementation::SingleConnection:
				return make_stateless <DbConnectSingle> (driver, std::ref (url), std::ref (user),
					std::ref (password))->_this ();
			case Implementation::ConnectionPool:
				return make_stateless <DbConnectPool> (driver, std::ref (url), std::ref (user),
					std::ref (password))->_this ();
		}
	}
};

}

NIRVANA_EXPORT_OBJECT (_exp_Test_db_connect_factory, Test::Static_db_connect_factory)
