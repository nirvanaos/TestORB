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
#include <CORBA/Server.h>
#include <IDL/Test_I1_factory_s.h>
#include "I1_factory_sysdomain.h"
#include "I1_factory_dynamic.h"

using namespace Nirvana;
using namespace CORBA;

namespace Test {

class I1_factory_sysdomain :
	public servant_traits <I1_factory>::ServantStatic <I1_factory_sysdomain>
{
public:
#ifndef LEGACY_CORBA_CPP
	static I1::_ref_type create (Long addendum)
	{
		SysDomain::_ref_type sys_domain = SysDomain::_narrow (g_ORB->resolve_initial_references ("SysDomain"));
		ProtDomain::_ref_type prot_domain = sys_domain->prot_domain ();
		I1_factory::_ref_type factory = I1_factory::_narrow (prot_domain->bind (StaticId <I1_factory_dynamic>::static_id_));
		return factory->create (addendum);
	}
#else
	static I1_ptr create (Long addendum)
	{
		Object_var osd = g_ORB->resolve_initial_references ("SysDomain");
		SysDomain_var sys_domain = SysDomain::_narrow (osd);
		ProtDomain_var prot_domain = sys_domain->prot_domain ();
		Object_var of = prot_domain->bind (StaticId <I1_factory_dynamic>::static_id_);
		I1_factory_var factory = I1_factory::_narrow (of);
		return factory->create (addendum);
	}
#endif
};

}

NIRVANA_STATIC_EXP (Test, I1_factory_sysdomain)
