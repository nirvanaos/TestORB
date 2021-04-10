// This file will be generated by the IDL compiler in the future.
#include "Test_I3_factory_s.h"
#include <CORBA/Proxy/Proxy.h>

namespace CORBA {
namespace Nirvana {

template <>
struct ProxyTraits <::Test::I3_factory>
{
	static const Operation operations_ [];
	static const Char* const interfaces_ [];

	// I3_ptr create (Long addendum);

	struct create_in
	{
		Long addendum;
	};
	static const Parameter create_in_params_ [1];

	struct create_out
	{
		Interface* _ret;
	};

	static void create_request (::Test::I3_factory_ptr _servant,
		IORequest_ptr _call,
		::Nirvana::ConstPointer _in_params,
		Unmarshal_var& _u,
		::Nirvana::Pointer _out_params)
	{
		const create_in& _in = *(const create_in*)_in_params;

		I_var <::Test::I3> _ret = _servant->create (_in.addendum);

		// Marshal output
		create_out& _out = *(create_out*)_out_params;
		Marshal_ptr _m = _call->marshaler ();
		_marshal_out (_ret, _m, _out._ret);
	}
};

IMPLEMENT_PROXY_FACTORY (::Test, I3_factory);

template <>
class Proxy <::Test::I3_factory> :
	public ProxyBase <::Test::I3_factory>
{
	typedef ProxyBase <::Test::I3_factory> Base;
	typedef ProxyTraits <::Test::I3_factory> Traits;
public:
	Proxy (IOReference_ptr proxy_manager, CORBA::UShort interface_idx) :
		Base (proxy_manager, interface_idx)
	{}

	::Test::I3_var create (Long addendum) const
	{
		Traits::create_in _in{ addendum };
		Traits::create_out _out;
		Marshal_var _m;
		Unmarshal_var _u = _target ()->call (CORBA::Nirvana::OperationIndex{ _interface_idx (), 0 },
			&_in, sizeof (_in), _m, &_out, sizeof (_out));
		::Test::I3_var _ret;
		_unmarshal (_out._ret, _u, _ret);
		return _ret;
	}
};

const Parameter ProxyTraits <::Test::I3_factory>::create_in_params_ [1] = {
	{ "addendum", Type <Long>::type_code }
};

const Operation ProxyTraits <::Test::I3_factory>::operations_ [] = {
	{ "create", { create_in_params_, countof (create_in_params_) }, {0, 0}, TypeI < ::Test::I3>::type_code, RqProcWrapper <::Test::I3_factory, create_request> }
};

const Char* const ProxyTraits <::Test::I3_factory>::interfaces_ [] = {
	::Test::I3_factory::repository_id_
};

template <>
const InterfaceMetadata ProxyFactoryImpl <::Test::I3_factory>::metadata_ = {
	{ProxyTraits <::Test::I3_factory>::interfaces_, countof (ProxyTraits <::Test::I3_factory>::interfaces_)},
	{ProxyTraits <::Test::I3_factory>::operations_, countof (ProxyTraits <::Test::I3_factory>::operations_)}
};

}
}

NIRVANA_EXPORT (_exp_Test_I3_factory_ProxyFactory, Test::I3_factory::repository_id_, CORBA::AbstractBase, CORBA::Nirvana::ProxyFactoryImpl <Test::I3_factory>)
