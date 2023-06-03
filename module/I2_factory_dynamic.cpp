#include <CORBA/Server.h>
#include <IDL/Test_I2_s.h>
#include <I2_factory_dynamic.h>
#include "DynamicI2.h"

using namespace CORBA;
using namespace PortableServer;

namespace Test {

class I2_factory_dynamic :
	public CORBA::servant_traits <I2_factory>::ServantStatic <I2_factory_dynamic>
{
public:
	static
#ifdef LEGACY_CORBA_CPP
		I2::_var_type
#else
		I2::_ref_type
#endif
		create (Long addendum)
	{
#ifdef LEGACY_CORBA_CPP
		{
			Servant serv = new DynamicI2 (addendum);
			release (serv);
		}
#endif
		servant_reference <DynamicI2> serv = make_reference <DynamicI2> (addendum);
		// Direct conversion to ServantBase must be available
		Servant s = serv;
		assert (s);

		// Return I1 proxy.
		return serv->_this ();
	}
};

}

