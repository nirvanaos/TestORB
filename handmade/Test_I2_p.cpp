// This file will be generated by the IDL compiler in the future.
#include <CORBA/Proxy/Proxy.h>
#include "Test_I2_s.h"

namespace CORBA {
namespace Internal {

IMPLEMENT_PROXY_FACTORY (::Test::, I2);

template <>
struct ProxyTraits < ::Test::I2>
{
	static const Operation operations_ [];
	static const Char* const interfaces_ [];

	// long op2 (long p1);

	typedef Long op2_in;
	typedef Long op2_out;
	static const Parameter op2_in_params_ [1];

	static void op2_request (::Test::I2_ptr _servant,
		IORequest_ptr _call,
		::Nirvana::ConstPointer _in_params,
		Unmarshal_var& _u,
		::Nirvana::Pointer _out_params)
	{
		const op2_in& _in = *(const op2_in*)_in_params;
		op2_out& _out = *(op2_out*)_out_params;
		_out = _servant->op2 (_in);
	}
};

template <>
class Proxy < ::Test::I2> :
	public ProxyBase < ::Test::I2>
{
	typedef ProxyBase < ::Test::I2> Base;
	typedef ProxyTraits < ::Test::I2> Traits;
public:
	Proxy (IOReference_ptr proxy_manager, CORBA::UShort interface_idx) :
		Base (proxy_manager, interface_idx)
	{}

	Long op2 (Long p1) const
	{
		Traits::op2_out _out;
		Marshal_var _m;
		_target ()->call (_make_op_idx (0),
			&p1, sizeof (p1), _m, &_out, sizeof (_out));
		return _out;
	}
};

const Parameter ProxyTraits < ::Test::I2>::op2_in_params_ [1] = {
	{ "p1", Type <Long>::type_code }
};

const Operation ProxyTraits < ::Test::I2>::operations_ [] = {
	{ "op2", { op2_in_params_, countof (op2_in_params_) }, {0, 0}, Type <Long>::type_code, RqProcWrapper < ::Test::I2, op2_request> }
};

const Char* const ProxyTraits < ::Test::I2>::interfaces_ [] = {
	::Test::I2::repository_id_
};

template <>
const InterfaceMetadata ProxyFactoryImpl < ::Test::I2>::metadata_ = {
	{ProxyTraits < ::Test::I2>::interfaces_, countof (ProxyTraits < ::Test::I2>::interfaces_)},
	{ProxyTraits < ::Test::I2>::operations_, countof (ProxyTraits < ::Test::I2>::operations_)}
};

}
}

namespace Test {
typedef CORBA::Internal::ProxyFactoryImpl <I2> _I2_ProxyFactory;
}

NIRVANA_EXPORT (_exp_Test_I2_ProxyFactory, Test::I2::repository_id_, CORBA::AbstractBase, CORBA::Internal::ProxyFactoryImpl <Test::I2>)
