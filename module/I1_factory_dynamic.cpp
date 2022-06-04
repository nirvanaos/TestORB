#include <CORBA/Server.h>
#include <IDL/Test_I1_factory_s.h>
#include "I1_factory_dynamic.h"
#include "DynamicI1.h"

using namespace PortableServer;
using namespace CORBA;

namespace Test {

class I1_factory_dynamic :
	public CORBA::servant_traits <I1_factory>::ServantStatic <I1_factory_dynamic>
{
public:
	static
#ifdef LEGACY_CORBA_CPP
		I1::_var_type
#else
		I1::_ref_type
#endif
	create (Long addendum)
	{
#ifdef LEGACY_CORBA_CPP
		{
			Servant serv = new DynamicI1 (addendum);
			release (serv);
		}
#endif
		servant_reference <DynamicI1> serv = make_reference <DynamicI1> (addendum);
		// Direct conversion to ServantBase must be available
		Servant s = serv;
		assert (s);

		// Return I1 proxy.
		return serv->_this ();
	}
};

}

NIRVANA_STATIC_EXP (Test, I1_factory_dynamic)
