// This file will be generated by the IDL compiler in the future.
#ifndef IDL_TEST_I1_H_
#define IDL_TEST_I1_H_

#include <CORBA/CORBA.h>

// typedef sequence <long> SeqLong;

namespace Test {

typedef std::vector <CORBA::Long> SeqLong;
class _TD_SeqLong;
extern const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_SeqLong;

}

namespace CORBA {
namespace Nirvana {

template <>
struct Type < ::Test::_TD_SeqLong> : Type <CORBA::Nirvana::Sequence <CORBA::Long, 0> >
{
	static TypeCode_ptr type_code ()
	{
		return ::Test::_tc_SeqLong;
	}
};

template <>
const char RepIdOf < ::Test::_TD_SeqLong>::repository_id_ [] = "IDL:Test/SeqLong:1.0";

}
}

namespace Test {

typedef CORBA::Nirvana::Type <_TD_SeqLong>::C_var SeqLong_var;
typedef CORBA::Nirvana::Type <_TD_SeqLong>::C_out SeqLong_out;
typedef CORBA::Nirvana::Type <_TD_SeqLong>::C_inout SeqLong_inout;

}

// MyException

namespace Test {

class MyException : public ::CORBA::UserException
{
public:
	DECLARE_EXCEPTION (MyException);

	::CORBA::Nirvana::Type <std::string>::Member_ret param () const
	{
		return _data.param;
	}

	void param (::CORBA::Nirvana::Type <std::string>::C_in val)
	{
		_data.param = val;
	}

	::CORBA::Nirvana::Type <bool>::Member_ret bparam () const
	{
		return _data.bparam;
	}

	void bparam (::CORBA::Nirvana::Type <bool>::C_in val)
	{
		_data.bparam = val;
	}

	struct _Data
	{
		::CORBA::Nirvana::Type <std::string>::Member_type param;
		::CORBA::Nirvana::Type <bool>::Member_type bparam;
	};

private:
	virtual void* __data () NIRVANA_NOEXCEPT
	{
		return &_data;
	}

	_Data _data;
};

extern const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_MyException;

}

namespace CORBA {
namespace Nirvana {

template <>
const Char RepIdOf < ::Test::MyException>::repository_id_ [] = "IDL:Test/MyException:1.0";

template <>
struct ABI < ::Test::MyException::_Data>
{
	Type <Type <std::string>::Member_type>::ABI_type param;
	Type <Type <bool>::Member_type>::ABI_type bparam;
};

template <>
struct Type <::Test::MyException::_Data> :
	TypeVarLen <::Test::MyException::_Data, Type <Type <std::string>::Member_type>::has_check>
{
	static void check (const ABI_type& val)
	{
		Type <Type <std::string>::Member_type>::check (val.param);
		Type <Type <bool>::Member_type>::check (val.bparam);
	}

	static void marshal_in (const Var_type& src, Marshal_ptr marshaler, ABI_type& dst)
	{
		_marshal_in (src.param, marshaler, dst.param);
		_marshal_in (src.bparam, marshaler, dst.bparam);
	}

	static void marshal_out (Var_type& src, Marshal_ptr marshaler, ABI_type& dst)
	{
		_marshal_out (src.param, marshaler, dst.param);
		_marshal_out (src.bparam, marshaler, dst.bparam);
	}

	static void unmarshal (const ABI_type& src, Unmarshal_ptr unmarshaler, Var_type& dst)
	{
		_unmarshal (src.param, unmarshaler, dst.param);
		_unmarshal (src.bparam, unmarshaler, dst.bparam);
	}
};

}
}

// struct MyStruct

namespace Test {

class MyStruct
{
public:
	MyStruct () :
		_l_member (0)
	{}
	MyStruct (const MyStruct&) = default;
	MyStruct (MyStruct&&) = default;
	MyStruct& operator = (const MyStruct&) = default;
	MyStruct& operator = (MyStruct&&) = default;

	::CORBA::Nirvana::Type <std::wstring>::Member_ret ws_member () const
	{
		return _ws_member;
	}
	void ws_member (::CORBA::Nirvana::Type <std::wstring>::C_in val)
	{
		_ws_member = val;
	}

	::CORBA::Nirvana::Type <int32_t>::Member_ret l_member () const
	{
		return _l_member;
	}
	void l_member (::CORBA::Nirvana::Type <int32_t>::C_in val)
	{
		_l_member = val;
	}

private:
	friend struct ::CORBA::Nirvana::Type <MyStruct>;
	::CORBA::Nirvana::Type <std::wstring>::Member_type _ws_member;
	::CORBA::Nirvana::Type <int32_t>::Member_type _l_member;
};
extern const ::Nirvana::ImportInterfaceT < ::CORBA::TypeCode> _tc_MyStruct;

}

namespace CORBA {
namespace Nirvana {

template <>
const char RepIdOf < ::Test::MyStruct>::repository_id_ [] = "IDL:Test/MyStruct:1.0";

template <>
struct ABI < ::Test::MyStruct>
{
	Type <Type < std::wstring>::Member_type>::ABI_type ws_member;
	Type <Type < int32_t>::Member_type>::ABI_type l_member;
};

template <>
struct Type < ::Test::MyStruct> : TypeVarLen < ::Test::MyStruct,
	Type <Type < std::wstring>::Member_type>::has_check>
{
	static void check (const ABI_type& val)
	{
		Type <Type < std::wstring>::Member_type>::check (val.ws_member);
		Type <Type < int32_t>::Member_type>::check (val.l_member);
	}

	static TypeCode_ptr type_code ()
	{
		return ::Test::_tc_MyStruct;
	}

	static void marshal_in (const Var_type& src, Marshal_ptr marshaler, ABI_type& dst)
	{
		_marshal_in (src._ws_member, marshaler, dst.ws_member);
		_marshal_in (src._l_member, marshaler, dst.l_member);
	}

	static void marshal_out (Var_type& src, Marshal_ptr marshaler, ABI_type& dst)
	{
		_marshal_out (src._ws_member, marshaler, dst.ws_member);
		_marshal_out (src._l_member, marshaler, dst.l_member);
	}

	static void unmarshal (const ABI_type& src, Unmarshal_ptr unmarshaler, Var_type& dst)
	{
		_unmarshal (src.ws_member, unmarshaler, dst._ws_member);
		_unmarshal (src.l_member, unmarshaler, dst._l_member);
	}
};

}
}

// interface I1

namespace Test {

class I1;
typedef CORBA::Nirvana::I_ptr <I1> I1_ptr;
typedef CORBA::Nirvana::I_var <I1> I1_var;
typedef CORBA::Nirvana::I_out <I1> I1_out;

extern const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_I1;

}

namespace CORBA {
namespace Nirvana {

template <>
struct Type <I_var < ::Test::I1> > : TypeObject < ::Test::I1>
{
	static TypeCode_ptr type_code ()
	{
		return ::Test::_tc_I1;
	}
};

template <>
struct Definitions < ::Test::I1>
{
	// Interface definitions will be here
};

BRIDGE_BEGIN (::Test::I1, "IDL:Test/I1:1.0")
BASE_STRUCT_ENTRY (Object, CORBA_Object)
BRIDGE_EPV
Long (*op1) (Bridge < ::Test::I1>*, Long p1, Interface*);
void (*throw_no_implement) (Bridge < ::Test::I1>*, Interface*);
Interface* (*object_op) (Bridge < ::Test::I1>*, Interface* in_obj, Interface** out_obj, Interface** inout_obj, Interface*);
ABI_ret <String> (*string_op) (Bridge < ::Test::I1>*, ABI_in <String> in_s, ABI_out <String> out_s, ABI_inout <String> inout_s, Interface*);
Type <::Test::_TD_SeqLong>::ABI_ret (*seq_op) (Bridge < ::Test::I1>*, Type <::Test::_TD_SeqLong>::ABI_in in_s, Type <::Test::_TD_SeqLong>::ABI_out out_s, Type <::Test::_TD_SeqLong>::ABI_inout inout_s, Interface*);
ABI_ret <Any> (*any_op) (Bridge < ::Test::I1>*, ABI_in <Any>, ABI_out <Any>, ABI_inout <Any>, Interface*);
BRIDGE_END ()

template <class T>
class Client <T, ::Test::I1> :
	public T
{
public:
	Long op1 (Long p1);
	void throw_no_implement ();
	::Test::I1_var object_op (I_in < ::Test::I1> in_obj, I_out < ::Test::I1> out_obj, I_inout < ::Test::I1> inout_obj);
	std::string string_op (String_in, String_out, String_inout);
	::Test::SeqLong seq_op (Type <::Test::_TD_SeqLong>::C_in in_s, Type <::Test::_TD_SeqLong>::C_out out_s, Type <::Test::_TD_SeqLong>::C_inout inout_s);
	Any any_op (Any_in, Any_out, Any_inout);
};

template <class T>
Long Client <T, ::Test::I1>::op1 (Long p1)
{
	Environment _env;
	Bridge < ::Test::I1>& _b (T::_get_bridge (_env));
	Long _ret = (_b._epv ().epv.op1) (&_b, p1, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, ::Test::I1>::throw_no_implement ()
{
	Environment _env;
	Bridge < ::Test::I1>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.throw_no_implement) (&_b, &_env);
	_env.check ();
}

template <class T>
::Test::I1_var Client <T, ::Test::I1>::object_op (I_in < ::Test::I1> in_obj, I_out < ::Test::I1> out_obj, I_inout < ::Test::I1> inout_obj)
{
	Environment _env;
	Bridge < ::Test::I1>& _b (T::_get_bridge (_env));
	I_ret < ::Test::I1> _ret = (_b._epv ().epv.object_op) (&_b, &in_obj, &out_obj, &inout_obj, &_env);
	_env.check ();
	return _ret;
}

template <class T>
std::string Client <T, ::Test::I1>::string_op (CORBA::String_in in_s, CORBA::String_out out_s, CORBA::String_inout inout_s)
{
	Environment _env;
	Bridge < ::Test::I1>& _b (T::_get_bridge (_env));
	Type <String>::C_ret _ret = (_b._epv ().epv.string_op) (&_b, &in_s, &out_s, &inout_s, &_env);
	_env.check ();
	return _ret;
}

template <class T>
::Test::SeqLong Client <T, ::Test::I1>::seq_op (Type <::Test::_TD_SeqLong>::C_in in_s, Type <::Test::_TD_SeqLong>::C_out out_s, Type <::Test::_TD_SeqLong>::C_inout inout_s)
{
	Environment _env;
	Bridge < ::Test::I1>& _b (T::_get_bridge (_env));
	Type <::Test::_TD_SeqLong>::C_ret _ret ((_b._epv ().epv.seq_op) (&_b, &in_s, &out_s, &inout_s, &_env));
	_env.check ();
	return _ret;
}

template <class T>
Any Client <T, ::Test::I1>::any_op (Any_in in_any, Any_out out_any, Any_inout inout_any)
{
	Environment _env;
	Bridge < ::Test::I1>& _b (T::_get_bridge (_env));
	Type <Any>::C_ret _ret = (_b._epv ().epv.any_op) (&_b, &in_any, &out_any, &inout_any, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Test {

class I1 : public CORBA::Nirvana::ClientInterface <I1, ::CORBA::Object>
{};

}

#endif
