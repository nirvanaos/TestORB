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
#include <CORBA/CORBA.h>
#include "ImplComp.h"

using namespace CORBA;

namespace Test {

class Static_comp_factory_dynamic :
	public servant_traits <Comp_factory>::ServantStatic <Static_comp_factory_dynamic>
{
public:
#ifdef LEGACY_CORBA_CPP

	static Comp_ptr create (Long addendum)
	{
		Comp_var comp;
		PortableServer::Servant_var <ImplComp> (new ImplComp (addendum, comp));
		return comp._retn ();
	}

#else

	static Comp::_ref_type create (Long addendum)
	{
		Comp::_ref_type comp;
		make_reference <ImplComp> (addendum, std::ref (comp));
		return comp;
	}

#endif
};

class Static_comp_factory_portable :
	public servant_traits <Comp_factory>::ServantStatic <Static_comp_factory_portable>
{
public:
#ifdef LEGACY_CORBA_CPP

	static Comp_ptr create (Long addendum)
	{
		Comp_var comp;
		PortableServer::Servant_var <ImplCompPOA> (new ImplCompPOA (addendum, comp));
		return comp._retn ();
	}

#else

	static Comp::_ref_type create (Long addendum)
	{
		Comp::_ref_type comp;
		make_reference <ImplCompPOA> (addendum, std::ref (comp));
		return comp;
	}

#endif
};

}

NIRVANA_EXPORT_OBJECT (_exp_Test_comp_factory_dynamic, Test::Static_comp_factory_dynamic)
NIRVANA_EXPORT_OBJECT (_exp_Test_comp_factory_portable, Test::Static_comp_factory_portable)

