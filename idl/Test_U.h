// This file was generated from "Test_U.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_TEST_U_H_
#define IDL_TEST_U_H_
#include "Test_I1.h"

namespace Test {

class S
{
public:

	S () NIRVANA_NOEXCEPT :
		_len (0)
	{}
	explicit S (::CORBA::Internal::Type < ::CORBA::Long>::Var len) :
		_len (std::move (len))
	{}

	::CORBA::Internal::Type < ::CORBA::Long>::ConstRef len () const
	{
		return _len;
	}
	::CORBA::Long& len ()
	{
		return _len;
	}
	void len (::CORBA::Internal::Type < ::CORBA::Long>::ConstRef val)
	{
		_len = val;
	}

private:
	friend struct ::CORBA::Internal::Type <S>;
	::CORBA::Long _len;
};
extern const ::Nirvana::ImportInterfaceT < ::CORBA::TypeCode> _tc_S;

}

namespace CORBA {
namespace Internal {

template <>
const Char RepIdOf < ::Test::S>::id [] = "IDL:Test/S:1.0";

template <>
struct Type < ::Test::S> :
	TypeFixLen < ::Test::S>
{
	static const bool has_check = false;
	static void check (const ABI& val);
	static void byteswap (Var&) NIRVANA_NOEXCEPT;

	static I_ptr <TypeCode> type_code ()
	{
		return ::Test::_tc_S;
	}

};

}
}

namespace Test {

class U
{
public:
	U () NIRVANA_NOEXCEPT;
	~U ()
	{
		_destruct ();
	}

	void _d (::CORBA::Long d);
	::CORBA::Long _d () const NIRVANA_NOEXCEPT
	{
		return __d;
	}

	::CORBA::Internal::Type < ::CORBA::Long>::ConstRef x () const
	{
		return const_cast <U&> (*this).x ();
	}
	::CORBA::Long& x ();
	::CORBA::Internal::Type < ::CORBA::Internal::String>::ConstRef z () const
	{
		return const_cast <U&> (*this).z ();
	}
	::CORBA::Internal::String& z ();
	void z (::CORBA::Internal::Type < ::CORBA::Internal::String>::Var&& val);
	::CORBA::Internal::Type <S>::ConstRef w () const
	{
		return const_cast <U&> (*this).w ();
	}
	S& w ();
	::CORBA::Internal::Type <I1>::ConstRef obj () const
	{
		return const_cast <U&> (*this).obj ();
	}
	::CORBA::Internal::Type <I1>::Var& obj ();
	void obj (::CORBA::Internal::Type <I1>::Var&& val);
private:
	void _destruct () NIRVANA_NOEXCEPT;
	static ::CORBA::Long _switch (::CORBA::Long d) NIRVANA_NOEXCEPT;

	::CORBA::Long __d;
	union _U
	{
		_U () {}
		~_U () {}

		::CORBA::Long _x;
		::CORBA::Internal::String _z;
		S _w;
		::CORBA::Internal::Type <I1>::Var _obj;
	} _u;
};
extern const ::Nirvana::ImportInterfaceT < ::CORBA::TypeCode> _tc_U;

class UFixed
{
public:
	UFixed () NIRVANA_NOEXCEPT :
		__d (3)
	{}
	~UFixed ()
	{
		_destruct ();
	}

	void _d (::CORBA::Long d);
	::CORBA::Long _d () const NIRVANA_NOEXCEPT
	{
		return __d;
	}

	void _default () NIRVANA_NOEXCEPT
	{
		_destruct ();
		__d = 3;
	}

	::CORBA::Internal::Type < ::CORBA::Long>::ConstRef x () const
	{
		return const_cast <UFixed&> (*this).x ();
	}
	::CORBA::Long& x ();
	::CORBA::Internal::Type < ::CORBA::Double>::ConstRef d () const
	{
		return const_cast <UFixed&> (*this).d ();
	}
	::CORBA::Double& d ();
private:
	void _destruct () NIRVANA_NOEXCEPT
	{}
	static ::CORBA::Long _switch (::CORBA::Long d) NIRVANA_NOEXCEPT;

	::CORBA::Long __d;
	union _U
	{
		_U () {}
		~_U () {}

		::CORBA::Long _x;
		::CORBA::Double _d;
	} _u;
};
extern const ::Nirvana::ImportInterfaceT < ::CORBA::TypeCode> _tc_UFixed;

class Z
{
public:
	Z () NIRVANA_NOEXCEPT :
		__d (false)
	{}
	~Z ()
	{
		_destruct ();
	}

	void _d (::CORBA::Boolean d);
	::CORBA::Boolean _d () const NIRVANA_NOEXCEPT
	{
		return __d;
	}

	void _default () NIRVANA_NOEXCEPT
	{
		_destruct ();
		__d = false;
	}

	::CORBA::Internal::Type < ::CORBA::Short>::ConstRef s () const
	{
		return const_cast <Z&> (*this).s ();
	}
	::CORBA::Short& s ();
private:
	void _destruct () NIRVANA_NOEXCEPT
	{}
	static ::CORBA::Boolean _switch (::CORBA::Boolean d) NIRVANA_NOEXCEPT
	{
		return d;
	}

	::CORBA::Boolean __d;
	union _U
	{
		_U () {}
		~_U () {}

		::CORBA::Short _s;
	} _u;
};
extern const ::Nirvana::ImportInterfaceT < ::CORBA::TypeCode> _tc_Z;

enum class Tag : ::CORBA::Internal::ABI_enum
{
	First,
	Second
};
extern const ::Nirvana::ImportInterfaceT < ::CORBA::TypeCode> _tc_Tag;

}

namespace CORBA {
namespace Internal {

template <>
const Char RepIdOf < ::Test::Tag>::id [] = "IDL:Test/Tag:1.0";

template <>
struct Type < ::Test::Tag> : public TypeEnum < ::Test::Tag, ::Test::Tag::Second>
{

	static I_ptr <TypeCode> type_code ()
	{
		return ::Test::_tc_Tag;
	}

};

}
}

namespace Test {

class Tagged
{
public:
	Tagged () NIRVANA_NOEXCEPT :
		__d (Tag::First)
	{}
	~Tagged ()
	{
		_destruct ();
	}

	void _d (Tag d);
	Tag _d () const NIRVANA_NOEXCEPT
	{
		return __d;
	}

	void _default () NIRVANA_NOEXCEPT
	{
		_destruct ();
		__d = Tag::First;
	}

	::CORBA::Internal::Type < ::CORBA::Short>::ConstRef s () const
	{
		return const_cast <Tagged&> (*this).s ();
	}
	::CORBA::Short& s ();
private:
	void _destruct () NIRVANA_NOEXCEPT
	{}
	static Tag _switch (Tag d) NIRVANA_NOEXCEPT;

	Tag __d;
	union _U
	{
		_U () {}
		~_U () {}

		::CORBA::Short _s;
	} _u;
};
extern const ::Nirvana::ImportInterfaceT < ::CORBA::TypeCode> _tc_Tagged;

}

#endif
