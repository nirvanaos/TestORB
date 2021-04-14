#ifndef IDL_TEST_I2_H_
#define IDL_TEST_I2_H_

namespace Test {

class I2;
typedef ::CORBA::Nirvana::I_ptr <I2> I2_ptr;
typedef ::CORBA::Nirvana::I_var <I2> I2_var;
typedef ::CORBA::Nirvana::I_out <I2> I2_out;

extern const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_I2;

}

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (::Test::I2, "IDL:Test/I2:1.0")
BASE_STRUCT_ENTRY (CORBA::Object, CORBA_Object)
BRIDGE_EPV
Long (*op2) (Bridge < ::Test::I2>*, Long p1, Interface*);
BRIDGE_END ()

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
struct Type <I_var < ::Test::I2> > : TypeObject < ::Test::I2>
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
