#include "I1_factory_portable.h"
#include "Test_I1_factory_s.h"
#include "PortableI1.h"

namespace Test {

class I1_factory_portable :
	public ::CORBA::servant_traits <I1_factory>::ServantStatic <I1_factory_portable>
{
public:
	static I1_var create (CORBA::Long addendum)
	{
		return PortableServer::Servant_var <PortableI1> (new PortableI1 (addendum))->_this ();
	}
};

}

NIRVANA_STATIC_EXP (Test, I1_factory_portable)
