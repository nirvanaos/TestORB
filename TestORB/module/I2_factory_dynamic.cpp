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
#include <IDL/Test_I2_s.h>
#include <I2_factory_dynamic.h>
#include "DynamicI2.h"

using namespace CORBA;
using namespace PortableServer;

namespace Test {

class I2_factory_dynamic :
	public servant_traits <I2_factory>::ServantStatic <I2_factory_dynamic>
{
public:
#ifdef LEGACY_CORBA_CPP

	static I2_ptr create (Long addendum)
	{
		Servant_var <DynamicI2> serv = new DynamicI2 (addendum);

		// Direct conversion to ServantBase must be available
		Servant s = serv;
		assert (s);

		return serv->_this ();
	}

#else

	static I2::_ref_type create (Long addendum)
	{
		servant_reference <DynamicI2> serv = make_reference <DynamicI2> (addendum);

		// Direct conversion to ServantBase must be available
		Servant s = serv;
		assert (s);

		return serv->_this ();
	}

#endif
};

}

NIRVANA_STATIC_EXP (Test, I2_factory_dynamic)
