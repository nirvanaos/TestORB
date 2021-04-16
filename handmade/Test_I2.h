#ifndef IDL_TEST_I2_H_
#define IDL_TEST_I2_H_

namespace Test {

class I2;
typedef ::CORBA::Nirvana::TypeItf <I2>::C_ptr I2_ptr;
typedef ::CORBA::Nirvana::TypeItf <I2>::C_var I2_var;
extern const ::Nirvana::ImportInterfaceT < ::CORBA::TypeCode> _tc_I2;

}

namespace CORBA {
namespace Nirvana {

NIRVANA_BRIDGE_BEGIN (::Test::I2, "IDL:Test/I2:1.0")
NIRVANA_BASE_ENTRY (CORBA::Object, CORBA_Object)
NIRVANA_BRIDGE_EPV
Long (*op2) (Bridge < ::Test::I2>*, Long p1, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ::Test::I2> :
	public T
{
public:
	Long op2 (Long p1);
};

template <class T>
Long Client <T, ::Test::I2>::op2 (Long p1)
{
	Environment _env;
	Bridge < ::Test::I2>& _b (T::_get_bridge (_env));
	Long _ret = (_b._epv ().epv.op2) (&_b, p1, &_env);
	_env.check ();
	return _ret;
}

template <>
struct Type < ::Test::I2> : TypeObject < ::Test::I2>
{
	static TypeCode_ptr type_code ()
	{
		return ::Test::_tc_I2;
	}
};

}
}

namespace Test {

class I2 : public CORBA::Nirvana::ClientInterface <I2, ::CORBA::Object>
{};

}

#endif
