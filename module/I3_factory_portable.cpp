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
#include "I3_factory_portable.h"
#include <IDL/Test_I3_factory_s.h>
#include "PortableI3.h"

using namespace CORBA;
using namespace PortableServer;

namespace Test {

class I3_factory_portable :
	public servant_traits <I3_factory>::ServantStatic <I3_factory_portable>
{
public:
#ifdef LEGACY_CORBA_CPP

	static I3::_ptr_type create (Long addendum)
	{
		Servant_var <PortableI3> serv = new PortableI3 (addendum);

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

	I3::_ref_type create (Long addendum)
	{
		servant_reference <PortableI3> serv = make_reference <PortableI3> (addendum);

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

NIRVANA_STATIC_EXP (Test, I3_factory_portable)
