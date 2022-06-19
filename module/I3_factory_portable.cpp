#include <CORBA/Server.h>
#include "I3_factory_portable.h"
#include <IDL/Test_I3_factory_s.h>
#include "PortableI3.h"

using namespace CORBA;

namespace Test {

class I3_factory_portable :
	public servant_traits <I3_factory>::ServantStatic <I3_factory_portable>
{
public:
	static
#ifdef LEGACY_CORBA_CPP
		I3::_ptr_type
#else
		I3::_ref_type
#endif
		create (Long addendum)
	{
#ifdef LEGACY_CORBA_CPP
		{
			LocalObject::_ptr_type lo = new PortableI3 (addendum);
			release (lo);
		}
#endif
		servant_reference <PortableI3> serv = make_reference <PortableI3> (addendum);
		// Direct conversion to LocalObject must be available
		LocalObject::_ptr_type lo = serv;
		// Conversion to Object must be available
		Object::_ptr_type obj = lo;
		assert (obj);
		// Object operations must be available
		bool is = lo->_is_a (CORBA::Internal::RepIdOf <I3>::id_);
		assert (is);

		// Return I3 proxy.
		return serv->_this ();
	}
};

}

NIRVANA_STATIC_EXP (Test, I3_factory_portable)
