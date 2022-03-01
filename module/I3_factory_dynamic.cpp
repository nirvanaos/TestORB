#include <CORBA/Server.h>
#include "I3_factory_dynamic.h"
#include <IDL/Test_I3_factory_s.h>
#include "DynamicI3.h"

namespace Test {

class I3_factory_dynamic :
	public CORBA::servant_traits <I3_factory>::ServantStatic <I3_factory_dynamic>
{
public:
	static
#ifdef LEGACY_CORBA_CPP
		I3::_ptr_type
#else
		I3::_ref_type
#endif
		create (CORBA::Long addendum)
	{
		return CORBA::make_reference <DynamicI3> (addendum)->_this ();
	}
};

}

NIRVANA_STATIC_EXP (Test, I3_factory_dynamic)
