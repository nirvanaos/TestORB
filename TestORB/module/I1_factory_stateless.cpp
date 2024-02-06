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
#include "Test_I1_factory_s.h"
#include "DynamicI1.h"

using namespace PortableServer;
using namespace CORBA;

namespace Test {

class Static_I1_factory_stateless :
	public CORBA::servant_traits <I1_factory>::ServantStatic <Static_I1_factory_stateless>
{
public:
	static I1::_ref_type create (Long addendum)
	{
		servant_reference <DynamicI1> serv = make_stateless <DynamicI1> (addendum);

		// Direct conversion to ServantBase must be available
		Servant s = serv;
		assert (s);

		// Return I1 proxy.
		return serv->_this ();
	}
};

}

NIRVANA_EXPORT_OBJECT (_exp_Test_I1_factory_stateless, Test::Static_I1_factory_stateless)
