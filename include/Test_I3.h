#ifndef IDL_TEST_I3_H_
#define IDL_TEST_I3_H_

#include "Test_I1.h"
#include "Test_I2.h"

namespace Test {

class I3;
typedef ::CORBA::Nirvana::I_ptr <I3> I3_ptr;
typedef ::CORBA::Nirvana::I_var <I3> I3_var;
typedef ::CORBA::Nirvana::I_out <I3> I3_out;

extern const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_I3;

}

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (::Test::I3, "IDL:Test/I3:1.0")
BASE_STRUCT_ENTRY (CORBA::Object, CORBA_Object)
BASE_STRUCT_ENTRY (::Test::I1, Test_I1)
BASE_STRUCT_ENTRY (::Test::I2, Test_I2)
BRIDGE_EPV
Long (*op3) (Bridge < ::Test::I3>*, Long p1, Interface*);
BRIDGE_END ()

template <class T>
class Client <T, ::Test::I3> :
	public T
{
public:
	Long op3 (Long p1);
};

template <class T>
Long Client <T, ::Test::I3>::op3 (Long p1)
{
	Environment _env;
	Bridge < ::Test::I3>& _b (T::_get_bridge (_env));
	Long _ret = (_b._epv ().epv.op3) (&_b, p1, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Test {

class I3 : public CORBA::Nirvana::ClientInterface <I3, I2, I1, ::CORBA::Object>
{};

}

#endif
