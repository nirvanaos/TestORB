#include <CORBA/Server.h>
#include "I3_factory_portable.h"
#include "Test_I3_factory_s.h"
#include "PortableI3.h"

namespace Test {

class I3_factory_portable :
	public CORBA::servant_traits <I3_factory>::ServantStatic <I3_factory_portable>
{
public:
	static I3_var create (CORBA::Long addendum)
	{
		return CORBA::make_reference <PortableI3> (addendum)->_this ();
	}
};

}

NIRVANA_STATIC_EXP (Test, I3_factory_portable)
