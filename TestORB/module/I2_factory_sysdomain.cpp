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
#include <Nirvana/Domains.h>
#include "IDL/Test_I2_s.h"

using namespace Nirvana;
using namespace CORBA;

namespace Test {

class Static_I2_factory_sysdomain :
	public servant_traits <I2_factory>::ServantStatic <Static_I2_factory_sysdomain>
{
public:
	static I2::_ref_type create (Long addendum)
	{
		SysDomain::_ref_type sys_domain = SysDomain::_narrow (g_ORB->resolve_initial_references ("SysDomain"));
		ProtDomain::_ref_type prot_domain = sys_domain->prot_domain ();
		I2_factory::_ref_type factory = I2_factory::_narrow (prot_domain->bind (Internal::StaticId <Static_I2_factory_dynamic>::id));
		return factory->create (addendum);
	}
};

}

NIRVANA_EXPORT_OBJECT (_exp_Test_I2_factory_sysdomain, Test::Static_I2_factory_sysdomain)
