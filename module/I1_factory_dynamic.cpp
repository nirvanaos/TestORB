#include <CORBA/Server.h>
#include <IDL/Test_I1_factory_s.h>
#include "I1_factory_dynamic.h"
#include "DynamicI1.h"

namespace Test {

class I1_factory_dynamic :
	public CORBA::servant_traits <I1_factory>::ServantStatic <I1_factory_dynamic>
{
public:
	static I1::_ref_type create (CORBA::Long addendum)
	{
		CORBA::servant_reference <DynamicI1> servant = CORBA::make_reference <DynamicI1> (addendum);
		servant->_add_ref ();
		unsigned long rcnt = servant->_refcount_value ();
		servant->_remove_ref ();
		return servant->_this ();
	}
};

}

NIRVANA_STATIC_EXP (Test, I1_factory_dynamic)
