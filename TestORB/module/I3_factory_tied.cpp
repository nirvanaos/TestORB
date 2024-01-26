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
#include "ImplI1.h"
#include "ImplI2.h"
#include "ImplI3.h"
#include "Test_I3_s.h"
#include "Test_I3_factory_s.h"

using namespace CORBA;
using namespace PortableServer;

namespace Test {

// Tied implementation

class TiedI3 :
	public ImplI1,
	public ImplI2,
	public ImplI3
{
public:
	TiedI3 (int32_t addendum) :
		addendum_ (addendum)
	{}

	int32_t op1 (int32_t p1) const
	{
		return p1 + addendum_;
	}

	int32_t addendum () const
	{
		return addendum_;
	}

	void addendum (int32_t a)
	{
		addendum_ = a;
	}

	int32_t op2 (int32_t p1) const
	{
		return p1 + 2 * addendum_;
	}

	int32_t op3 (int32_t p1) const
	{
		return p1 + 3 * addendum_;
	}

	int32_t aop (int32_t x) const
	{
		return x - addendum ();
	}

private:
	int32_t addendum_;
};

class Static_I3_factory_tied :
	public servant_traits <I3_factory>::ServantStatic <Static_I3_factory_tied>
{
public:
#ifdef LEGACY_CORBA_CPP

	static I3::_ptr_type create (Long addendum)
	{
		Servant_var <POA_Test::I3_tie <TiedI3> > serv = new POA_Test::I3_tie <TiedI3> (new TiedI3 (addendum));

		// Direct conversion to LocalObject must be available
		LocalObject::_ptr_type lo = serv;
		assert (lo);

		// Conversion to Object must be available
		Object::_ptr_type obj = lo;
		assert (obj);

		// Object operations must be available
		bool is = lo->_is_a (CORBA::Internal::RepIdOf <I3>::id);
		assert (is);

		return serv->_this ();
	}

#else

	static I3::_ref_type create (Long addendum)
	{
		auto serv = make_reference <servant_traits <I3>::tie_type <TiedI3> > (new TiedI3 (addendum));

		// Direct conversion to LocalObject must be available
		LocalObject::_ptr_type lo = serv;
		assert (lo);

		// Conversion to Object must be available
		Object::_ptr_type obj = lo;
		assert (obj);

		// Object operations must be available
		bool is = lo->_is_a (CORBA::Internal::RepIdOf <I3>::id);
		assert (is);

		return serv->_this ();
	}

#endif
};

#ifdef LEGACY_CORBA_CPP

class TiedDerivedI3 :
	public POA_Test::I3_tie <TiedI3>,
	public TiedI3
{
public:
	TiedDerivedI3 (Long addendum) :
		POA_Test::I3_tie <TiedI3> (static_cast <TiedI3&> (*this)),
		TiedI3 (addendum)
	{}
};

#else

class TiedDerivedI3 :
	public servant_traits <I3>::tie_type <TiedI3>,
	public TiedI3
{
public:
	TiedDerivedI3 (Long addendum) :
		servant_traits <I3>::tie_type <TiedI3> (static_cast <TiedI3&> (*this)),
		TiedI3 (addendum)
	{}
};

#endif

class Static_I3_tied_derived :
	public servant_traits <I3_factory>::ServantStatic <Static_I3_tied_derived>
{
public:
#ifdef LEGACY_CORBA_CPP

	static I3::_ptr_type create (Long addendum)
	{
		Servant_var <TiedDerivedI3> serv = new TiedDerivedI3 (addendum);

		// Direct conversion to LocalObject must be available
		LocalObject::_ptr_type lo = serv;
		assert (lo);

		// Conversion to Object must be available
		Object::_ptr_type obj = lo;
		assert (obj);

		// Object operations must be available
		bool is = lo->_is_a (CORBA::Internal::RepIdOf <I3>::id);
		assert (is);

		return serv->_this ();
	}

#else

	static I3::_ref_type create (Long addendum)
	{
		servant_reference <TiedDerivedI3> serv = make_reference <TiedDerivedI3> (addendum);

		// Direct conversion to LocalObject must be available
		LocalObject::_ptr_type lo = serv;
		assert (lo);

		// Conversion to Object must be available
		Object::_ptr_type obj = lo;
		assert (obj);

		// Object operations must be available
		bool is = lo->_is_a (CORBA::Internal::RepIdOf <I3>::id);
		assert (is);

		return serv->_this ();
	}

#endif
};

}

NIRVANA_EXPORT_OBJECT (_exp_Test_I3_factory_tied, Test::Static_I3_factory_tied)
NIRVANA_EXPORT_OBJECT (_exp_Test_I3_tied_derived, Test::Static_I3_tied_derived)
