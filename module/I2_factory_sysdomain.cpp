#include <Nirvana/Nirvana.h>
#include <Nirvana/Domains.h>
#include <CORBA/Server.h>
#include <IDL/Test_I2_s.h>
#include "I2_factory_sysdomain.h"
#include "I2_factory_dynamic.h"

using namespace Nirvana;
using namespace CORBA;

namespace Test {

class I2_factory_sysdomain :
	public CORBA::servant_traits <I2_factory>::ServantStatic <I2_factory_sysdomain>
{
public:
	static
#ifdef LEGACY_CORBA_CPP
		I2::_ptr_type
#else
		I2::_ref_type
#endif
		create (Long addendum)
	{
#ifdef LEGACY_CORBA_CPP
		Object_var obj (g_ORB->resolve_initial_references ("SysDomain"));
		SysDomain_var sys_domain = SysDomain::_narrow (obj);
#else
		SysDomain::_ref_type sys_domain = SysDomain::_narrow (g_ORB->resolve_initial_references ("SysDomain"));
#endif

#ifdef LEGACY_CORBA_CPP
		ProtDomain::_var_type
#else
		ProtDomain::_ref_type
#endif
			prot_domain = sys_domain->prot_domain ();

#ifdef LEGACY_CORBA_CPP
		obj = prot_domain->bind (StaticId <I2_factory_dynamic>::static_id_);
		I2_factory::_var_type factory = I2_factory::_narrow (obj);
#else
		I2_factory::_ref_type factory = I2_factory::_narrow (prot_domain->bind (StaticId <I2_factory_dynamic>::static_id_));
#endif

		return factory->create (addendum);
	}
};

}

NIRVANA_STATIC_EXP (Test, I2_factory_sysdomain)
