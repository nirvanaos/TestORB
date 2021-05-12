#include <CORBA/Server.h>
#include "Test_I1_factory_s.h"
#include "I1_factory_dynamic.h"
#include "DynamicI1.h"

namespace Test {

class I1_factory_dynamic :
	public CORBA::servant_traits <I1_factory>::ServantStatic <I1_factory_dynamic>
{
public:
	static I1::_ref_type create (CORBA::Long addendum)
	{
		return CORBA::make_reference <DynamicI1> (addendum)->_this ();
	}
};

}

NIRVANA_STATIC_EXP(Test, I1_factory_dynamic)
