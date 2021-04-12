// This file will be generated by the IDL compiler in the future.
#include "Test_I1_s.h"
#include <CORBA/Proxy/Proxy.h>

namespace CORBA {
namespace Nirvana {

template <>
const Char TypeCodeName < ::Test::_TD_SeqLong>::name_ [] = "SeqLong";

template <>
const Parameter TypeCodeMembers < ::Test::MyException>::members_ [] = {
	{ "param", Type <String>::type_code },
	{ "bparam", Type <Boolean>::type_code }
};

template <>
const Char TypeCodeName < ::Test::MyStruct>::name_ [] = "MyStruct";

template <>
const Parameter TypeCodeMembers < ::Test::MyStruct>::members_ [] = {
	{ "ws_member", Type <WString>::type_code },
	{ "l_member", Type <Long>::type_code }
};

IMPLEMENT_PROXY_FACTORY(::Test, I1);

template <>
struct ProxyTraits <::Test::I1>
{
	static const Operation operations_ [];
	static const Char* const interfaces_ [];

	// long op1 (long p1);

	typedef Long op1_in;
	typedef Long op1_out;
	static const Parameter op1_in_params_ [1];

	static void op1_request (::Test::I1_ptr _servant,
		IORequest_ptr _call,
		::Nirvana::ConstPointer _in_params,
		Unmarshal_var& _u,
		::Nirvana::Pointer _out_params)
	{
		const op1_in& _in = *(const op1_in*)_in_params;
		op1_out& _out = *(op1_out*)_out_params;
		_out = _servant->op1 (_in);
	}

	// void throw_no_implement ();

	static void throw_no_implement_request (::Test::I1_ptr _servant,
		IORequest_ptr _call,
		::Nirvana::ConstPointer _in_params,
		Unmarshal_var& _u,
		::Nirvana::Pointer _out_params)
	{
		_servant->throw_no_implement ();
	}

	// I1 object_op (I1 in_obj, out I1 out_obj, inout I1 inout_obj);

	struct object_op_in
	{
		Interface* in_obj;
		Interface* inout_obj;
	};
	static const Parameter object_op_in_params_ [2];

	struct object_op_out
	{
		Interface* out_obj;
		Interface* inout_obj;
		Interface* _ret;
	};
	static const Parameter object_op_out_params_ [2];

	static void object_op_request (::Test::I1_ptr _servant,
		IORequest_ptr _call,
		::Nirvana::ConstPointer _in_ptr,
		Unmarshal_var& _u,
		::Nirvana::Pointer _out_ptr)
	{
		const object_op_in& _in = *(const object_op_in*)_in_ptr;
			
		// out and inout params
		I_var <::Test::I1> out_obj;
		I_var <::Test::I1> inout_obj;
		I_var <::Test::I1> _ret;
		{
			// in params
			I_var <::Test::I1> in_obj;

			// Unmarshal in and inout
			_unmarshal (_in.in_obj, _u, in_obj);
			_unmarshal (_in.inout_obj, _u, inout_obj);

			// Release resources
			_u = Unmarshal::_nil ();

			// Call
			_ret = _servant->object_op (in_obj, out_obj, inout_obj);

			// Input params out of scope here
		}

		// Marshal output
		object_op_out& _out = *(object_op_out*)_out_ptr;
		Marshal_var _m = _call->marshaler ();
		_marshal_out (out_obj, _m, _out.out_obj);
		_marshal_out (inout_obj, _m, _out.inout_obj);
		_marshal_out (_ret, _m, _out._ret);
	}

	// string string_op (string in_s, out string out_s, inout string inout_s);

	struct string_op_in
	{
		ABI <String> in_s;
		ABI <String> inout_s;
	};
	static const Parameter string_op_in_params_ [2];

	struct string_op_out
	{
		ABI <String> out_s;
		ABI <String> inout_s;
		ABI <String> _ret;
	};
	static const Parameter string_op_out_params_ [2];

	static void string_op_request (::Test::I1_ptr _servant,
		IORequest_ptr _call,
		::Nirvana::ConstPointer _in_ptr,
		Unmarshal_var& _u,
		::Nirvana::Pointer _out_ptr)
	{
		const string_op_in& _in = *(const string_op_in*)_in_ptr;

		// out and inout params
		String out_s;
		String inout_s;
		String _ret;
		{
			// in params
			String in_s;

			// Unmarshal in and inout
			_unmarshal (_in.in_s, _u, in_s);
			_unmarshal (_in.inout_s, _u, inout_s);

			// Release resources
			_u = Unmarshal::_nil ();

			// Call
			_ret = _servant->string_op (in_s, out_s, inout_s);

			// Input params out of scope here
		}

		// Marshal output
		string_op_out& _out = *(string_op_out*)_out_ptr;
		Marshal_var _m = _call->marshaler ();
		_marshal_out (out_s, _m, _out.out_s);
		_marshal_out (inout_s, _m, _out.inout_s);
		_marshal_out (_ret, _m, _out._ret);
	}

	// SeqLong seq_op (SeqLong in_s, out SeqLong out_s, inout SeqLong inout_s);

	struct seq_op_in
	{
		Type <Sequence <Long> >::ABI_type in_s;
		Type <Sequence <Long> >::ABI_type inout_s;
	};
	static const Parameter seq_op_in_params_ [2];

	struct seq_op_out
	{
		Type <Sequence <Long> >::ABI_type out_s;
		Type <Sequence <Long> >::ABI_type inout_s;
		Type <Sequence <Long> >::ABI_type _ret;
	};
	static const Parameter seq_op_out_params_ [2];

	static void seq_op_request (::Test::I1_ptr _servant,
		IORequest_ptr _call,
		::Nirvana::ConstPointer _in_ptr,
		Unmarshal_var& _u,
		::Nirvana::Pointer _out_ptr)
	{
		const seq_op_in& _in = *(const seq_op_in*)_in_ptr;

		// out and inout params
		Test::SeqLong out_s;
		Test::SeqLong inout_s;
		Test::SeqLong _ret;
		{
			// in params
			Test::SeqLong in_s;

			// Unmarshal in and inout
			_unmarshal (_in.in_s, _u, in_s);
			_unmarshal (_in.inout_s, _u, inout_s);

			// Release resources
			_u = Unmarshal::_nil ();

			// Call
			_ret = _servant->seq_op (in_s, out_s, inout_s);

			// Input params out of scope here
		}

		// Marshal output
		seq_op_out& _out = *(seq_op_out*)_out_ptr;
		Marshal_var _m = _call->marshaler ();
		_marshal_out (out_s, _m, _out.out_s);
		_marshal_out (inout_s, _m, _out.inout_s);
		_marshal_out (_ret, _m, _out._ret);
	}

	// any any_op (any in_any, out any out_any, inout any inout_any);

	struct any_op_in
	{
		ABI <Any> in_any;
		ABI <Any> inout_any;
	};
	static const Parameter any_op_in_params_ [2];

	struct any_op_out
	{
		ABI <Any> out_any;
		ABI <Any> inout_any;
		ABI <Any> _ret;
	};
	static const Parameter any_op_out_params_ [2];

	static void any_op_request (::Test::I1_ptr _servant,
		IORequest_ptr _call,
		::Nirvana::ConstPointer _in_ptr,
		Unmarshal_var& _u,
		::Nirvana::Pointer _out_ptr)
	{
		const any_op_in& _in = *(const any_op_in*)_in_ptr;

		// out and inout params
		Any out_any;
		Any inout_any;
		Any _ret;
		{
			// in params
			Any in_any;

			// Unmarshal in and inout
			_unmarshal (_in.in_any, _u, in_any);
			_unmarshal (_in.inout_any, _u, inout_any);

			// Release resources
			_u = Unmarshal::_nil ();

			// Call
			_ret = _servant->any_op (in_any, out_any, inout_any);

			// Input params out of scope here
		}

		// Marshal output
		any_op_out& _out = *(any_op_out*)_out_ptr;
		Marshal_var _m = _call->marshaler ();
		_marshal_out (out_any, _m, _out.out_any);
		_marshal_out (inout_any, _m, _out.inout_any);
		_marshal_out (_ret, _m, _out._ret);
	}
};

template <>
class Proxy <::Test::I1> :
	public ProxyBase <::Test::I1>
{
	typedef ProxyBase <::Test::I1> Base;
	typedef ProxyTraits <::Test::I1> Traits;
public:
	Proxy (IOReference_ptr proxy_manager, CORBA::UShort interface_idx) :
		Base (proxy_manager, interface_idx)
	{}

	Long op1 (Long p1) const
	{
		Traits::op1_out _out;
		Marshal_var _m;
		_target ()->call (CORBA::Nirvana::OperationIndex{ _interface_idx (), 0 },
			&p1, sizeof (p1), _m, &_out, sizeof (_out));
		return _out;
	}

	void throw_no_implement () const
	{
		Marshal_var _m;
		_target ()->call (CORBA::Nirvana::OperationIndex{ _interface_idx (), 1 },
			0, 0, _m, 0, 0);
	}

	::Test::I1_var object_op (::Test::I1_ptr in_obj, ::Test::I1_var& out_obj, ::Test::I1_var& inout_obj) const
	{
		Traits::object_op_in _in;
		Marshal_var _m = _target ()->create_marshaler ();
		_marshal_in (in_obj, _m, _in.in_obj);
		_marshal_in (inout_obj, _m, _in.inout_obj);
		Traits::object_op_out _out;
		Unmarshal_var _u = _target ()->call (CORBA::Nirvana::OperationIndex{ _interface_idx (), 2 },
			&_in, sizeof (_in), _m, &_out, sizeof (_out));
		_unmarshal (_out.out_obj, _u, out_obj);
		_unmarshal (_out.inout_obj, _u, inout_obj);
		::Test::I1_var _ret;
		_unmarshal (_out._ret, _u, _ret);
		return _ret;
	}

	String string_op (const String& in_s, String& out_s, String& inout_s) const
	{
		Traits::string_op_in _in;
		Marshal_var _m = _target ()->create_marshaler ();
		_marshal_in (in_s, _m, _in.in_s);
		_marshal_in (inout_s, _m, _in.inout_s);
		Traits::string_op_out _out;
		Unmarshal_var _u = _target ()->call (CORBA::Nirvana::OperationIndex{ _interface_idx (), 3 },
			&_in, sizeof (_in), _m, &_out, sizeof (_out));
		_unmarshal (_out.out_s, _u, out_s);
		_unmarshal (_out.inout_s, _u, inout_s);
		String _ret;
		_unmarshal (_out._ret, _u, _ret);
		return _ret;
	}

	::Test::SeqLong seq_op (const ::Test::SeqLong& in_s, ::Test::SeqLong& out_s, ::Test::SeqLong& inout_s) const
	{
		Traits::seq_op_in _in;
		Marshal_var _m = _target ()->create_marshaler ();
		_marshal_in (in_s, _m, _in.in_s);
		_marshal_in (inout_s, _m, _in.inout_s);
		Traits::seq_op_out _out;
		Unmarshal_var _u = _target ()->call (CORBA::Nirvana::OperationIndex{ _interface_idx (), 4 },
			&_in, sizeof (_in), _m, &_out, sizeof (_out));
		_unmarshal (_out.out_s, _u, out_s);
		_unmarshal (_out.inout_s, _u, inout_s);
		Test::SeqLong _ret;
		_unmarshal (_out._ret, _u, _ret);
		return _ret;
	}

	Any any_op (const Any& in_any, Any& out_any, Any& inout_any) const
	{
		Traits::any_op_in _in;
		Marshal_var _m = _target ()->create_marshaler ();
		_marshal_in (in_any, _m, _in.in_any);
		_marshal_in (inout_any, _m, _in.inout_any);
		Traits::any_op_out _out;
		Unmarshal_var _u = _target ()->call (CORBA::Nirvana::OperationIndex{ _interface_idx (), 5 },
			&_in, sizeof (_in), _m, &_out, sizeof (_out));
		_unmarshal (_out.out_any, _u, out_any);
		_unmarshal (_out.inout_any, _u, inout_any);
		Any _ret;
		_unmarshal (_out._ret, _u, _ret);
		return _ret;
	}
};

const Parameter ProxyTraits <::Test::I1>::op1_in_params_ [1] = {
	{ "p1", Type <Long>::type_code }
};

const Parameter ProxyTraits <::Test::I1>::object_op_in_params_ [2] = {
	{ "in_obj", Type <I_var < ::Test::I1> >::type_code },
	{ "inout_obj", Type <I_var < ::Test::I1> >::type_code }
};

const Parameter ProxyTraits <::Test::I1>::object_op_out_params_ [2] = {
	{ "out_obj", Type <I_var < ::Test::I1> >::type_code },
	{ "inout_obj", Type <I_var < ::Test::I1> >::type_code }
};

const Parameter ProxyTraits <::Test::I1>::string_op_in_params_ [2] = {
	{ "in_s", Type <String>::type_code },
	{ "inout_s", Type <String>::type_code }
};

const Parameter ProxyTraits <::Test::I1>::string_op_out_params_ [2] = {
	{ "out_s", Type <String>::type_code },
	{ "inout_s", Type <String>::type_code }
};

const Parameter ProxyTraits <::Test::I1>::seq_op_in_params_ [2] = {
	{ "in_s", Type <Test::SeqLong>::type_code },
	{ "inout_s", Type <Test::SeqLong>::type_code }
};

const Parameter ProxyTraits <::Test::I1>::seq_op_out_params_ [2] = {
	{ "out_s", Type <Test::SeqLong>::type_code },
	{ "inout_s", Type <Test::SeqLong>::type_code }
};

const Parameter ProxyTraits <::Test::I1>::any_op_in_params_ [2] = {
	{ "in_any", Type <Any>::type_code },
	{ "inout_any", Type <Any>::type_code }
};

const Parameter ProxyTraits <::Test::I1>::any_op_out_params_ [2] = {
	{ "out_any", Type <Any>::type_code },
	{ "inout_any", Type <Any>::type_code }
};

const Operation ProxyTraits <::Test::I1>::operations_ [] = {
	{ "op1", { op1_in_params_, countof (op1_in_params_) }, {0, 0}, Type <Long>::type_code, RqProcWrapper <::Test::I1, op1_request> },
	{ "throw_no_implement", { 0, 0 }, {0, 0}, Type <void>::type_code, RqProcWrapper <::Test::I1, throw_no_implement_request> },
	{ "object_op", { object_op_in_params_, countof (object_op_in_params_) }, { object_op_out_params_, countof (object_op_out_params_) }, Type <I_var < ::Test::I1> >::type_code, RqProcWrapper <::Test::I1, object_op_request> },
	{ "string_op", { string_op_in_params_, countof (string_op_in_params_) }, { string_op_out_params_, countof (string_op_out_params_) }, Type <String>::type_code, RqProcWrapper <::Test::I1, string_op_request> },
	{ "seq_op", { seq_op_in_params_, countof (seq_op_in_params_) }, { seq_op_out_params_, countof (seq_op_out_params_) }, Type < ::Test::SeqLong>::type_code, RqProcWrapper <::Test::I1, seq_op_request> },
	{ "any_op", { any_op_in_params_, countof (any_op_in_params_) }, { any_op_out_params_, countof (any_op_out_params_) }, Type <Any>::type_code, RqProcWrapper <::Test::I1, any_op_request> }
};

const Char* const ProxyTraits <::Test::I1>::interfaces_ [] = {
	::Test::I1::repository_id_
};

template <>
const InterfaceMetadata ProxyFactoryImpl <::Test::I1>::metadata_ = {
	{ProxyTraits <::Test::I1>::interfaces_, countof (ProxyTraits <::Test::I1>::interfaces_)},
  {ProxyTraits <::Test::I1>::operations_, countof (ProxyTraits <::Test::I1>::operations_)}
};

}
}

NIRVANA_EXPORT (_exp_Test_TC_SeqLong, CORBA::Nirvana::RepIdOf <Test::_TD_SeqLong>::repository_id_, CORBA::TypeCode, CORBA::Nirvana::TypeCodeTypeDef < Test::_TD_SeqLong, CORBA::Nirvana::Sequence <CORBA::Long> >)
NIRVANA_EXPORT (_exp_Test_TC_MyException, CORBA::Nirvana::RepIdOf <Test::MyException>::repository_id_, CORBA::TypeCode, CORBA::Nirvana::TypeCodeException <Test::MyException, true>)
NIRVANA_EXPORT (_exp_Test_TC_MyStruct, CORBA::Nirvana::RepIdOf <Test::MyStruct>::repository_id_, CORBA::TypeCode, CORBA::Nirvana::TypeCodeStruct < Test::MyStruct>)
NIRVANA_EXPORT (_exp_Test_I1_ProxyFactory, Test::I1::repository_id_, CORBA::AbstractBase, CORBA::Nirvana::ProxyFactoryImpl <::Test::I1>)