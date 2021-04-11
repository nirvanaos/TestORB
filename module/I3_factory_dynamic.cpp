#include <CORBA/Server.h>
#include "I3_factory_dynamic.h"
#include "Test_I3_factory_s.h"
#include "DynamicI3.h"

namespace Test {

class I3_factory_dynamic :
	public CORBA::servant_traits <I3_factory>::ServantStatic <I3_factory_dynamic>
{
public:
	static I3_var create (CORBA::Long addendum)
	{
		return PortableServer::Servant_var <DynamicI3> (new DynamicI3 (addendum))->_this ();
	}
};

}

NIRVANA_STATIC_EXP (Test, I3_factory_dynamic)
