#include <CORBA/Server.h>
#include <IDL/Test_I1_factory_s.h>
#include "I1_factory_portable.h"
#include "PortableI1.h"

namespace Test {

class I1_factory_portable :
	public CORBA::servant_traits <I1_factory>::ServantStatic <I1_factory_portable>
{
public:
	static 
#ifdef LEGACY_CORBA_CPP
		I1::_ptr_type
#else
		I1::_ref_type
#endif
		create (CORBA::Long addendum)
	{
		return CORBA::make_reference <PortableI1> (addendum)->_this ();
	}
};

}

NIRVANA_STATIC_EXP (Test, I1_factory_portable)
