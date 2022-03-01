#include <CORBA/Server.h>
#include "I3_factory_dynamic.h"
#include <IDL/Test_I3_factory_s.h>
#include "DynamicI3.h"

namespace Test {

class I3_factory_dynamic :
	public CORBA::servant_traits <I3_factory>::ServantStatic <I3_factory_dynamic>
{
public:
	static I3::_ref_type create (CORBA::Long addendum)
	{
		CORBA::servant_reference <DynamicI3> servant = CORBA::make_reference <DynamicI3> (addendum);
		servant->_add_ref ();
		unsigned long rcnt = servant->_refcount_value ();
		servant->_remove_ref ();
		return servant->_this ();
	}
};

}

NIRVANA_STATIC_EXP (Test, I3_factory_dynamic)
