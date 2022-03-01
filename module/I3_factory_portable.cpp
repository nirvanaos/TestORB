#include <CORBA/Server.h>
#include "I3_factory_portable.h"
#include <IDL/Test_I3_factory_s.h>
#include "PortableI3.h"

namespace Test {

class I3_factory_portable :
	public CORBA::servant_traits <I3_factory>::ServantStatic <I3_factory_portable>
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
		return CORBA::make_reference <PortableI3> (addendum)->_this ();
	}
};

}

NIRVANA_STATIC_EXP (Test, I3_factory_portable)
