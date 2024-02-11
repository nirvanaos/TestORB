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
#include "IDL/Test_I1_factory_s.h"
#include "PortableI1.h"

using namespace PortableServer;
using namespace CORBA;

namespace Test {

class Static_I1_factory_portable :
	public CORBA::servant_traits <I1_factory>::ServantStatic <Static_I1_factory_portable>
{
public:
#ifdef LEGACY_CORBA_CPP

	static I1_ptr create (Long addendum)
	{
		Servant_var <PortableI1> serv = new PortableI1 (addendum);

		// Direct conversion to ServantBase must be available
		Servant s = serv;
		assert (s);

		return serv->_this ();
	}

#else

	static I1::_ref_type create (Long addendum)
	{
		servant_reference <PortableI1> serv = make_reference <PortableI1> (addendum);

		// Direct conversion to ServantBase must be available
		Servant s = serv;
		assert (s);

		// Return I1 proxy.
		return serv->_this ();
	}

#endif
};

}

NIRVANA_EXPORT_OBJECT (_exp_Test_I1_factory_portable, Test::Static_I1_factory_portable)
