#include "I3_portable.h"
#include "Test_I3_factory_s.h"
#include "PortableI3.h"

namespace Test {

class I3_portable :
	public ::CORBA::Nirvana::ServantStatic <I3_portable, I3_factory>
{
public:
	static I3_var create (CORBA::Long addendum)
	{
		return PortableServer::Servant_var <PortableI3> (new PortableI3 (addendum))->_this ();
	}
};

}

NIRVANA_STATIC_EXP (Test, I3_portable)
