#include "I1_dynamic.h"
#include "Test_I1_factory_s.h"
#include "DynamicI1.h"

namespace Test {

class I1_dynamic :
	public ::CORBA::Nirvana::ServantStatic <I1_dynamic, I1_factory>
{
public:
	static I1_var create (CORBA::Long addendum)
	{
		return PortableServer::Servant_var <DynamicI1> (new DynamicI1 (addendum))->_this ();
	}
};

}

NIRVANA_STATIC_EXP(Test, I1_dynamic)
