// This file was generated from "Test_U.idl"
// Nirvana IDL compiler version 1.0
#include <CORBA/CORBA.h>
#include <Nirvana/OLF.h>
#include "Test_U.h"

namespace CORBA {
namespace Internal {

void Type < ::Test::S>::check (const ABI & val)
{
}

}
}

NIRVANA_OLF_SECTION_N (0) extern const Nirvana::ImportInterfaceT <CORBA::TypeCode>
Test::_tc_S = { Nirvana::OLF_IMPORT_INTERFACE, CORBA::Internal::RepIdOf <Test::S>::id, CORBA::Internal::RepIdOf <CORBA::TypeCode>::id };

namespace Test {

U::U () NIRVANA_NOEXCEPT :
	__d (5)
{
	::CORBA::Internal::construct (_u._obj);
}

void U::_d (::CORBA::Long d)
{
	if (_switch (d) != _switch (__d))
		throw ::CORBA::BAD_PARAM ();
}

::CORBA::Long& x ()
{
	if (_swtich (__d) != 1)
		throw ::CORBA::BAD_PARAM ();
	return _u._x;
}
void x (::CORBA::Internal::Type < ::CORBA::Long>::ConstRef val)
{
	if (_swtich (__d) != 1) {
		_destruct ();
		::CORBA::Internal::_construct (_u._x, val);__d = 1;
	} else_u._x = val;}
::CORBA::Internal::String& z ()
{
	if (_swtich (__d) != 2)
		throw ::CORBA::BAD_PARAM ();
	return _u._z;
}
void z (::CORBA::Internal::Type < ::CORBA::Internal::String>::ConstRef val)
{
	if (_swtich (__d) != 2) {
		_destruct ();
		::CORBA::Internal::_construct (_u._z, val);__d = 2;
	} else_u._z = val;}
void z (::CORBA::Internal::Type < ::CORBA::Internal::String>::Var&& val)
{
	if (_swtich (__d) != 2) {
		_destruct ();
		::CORBA::Internal::_construct (_u._z, std::move (val));__d = 2;
	} else_u._z = std::move (val);}
S& w ()
{
	if (_swtich (__d) != 3)
		throw ::CORBA::BAD_PARAM ();
	return _u._w;
}
void w (::CORBA::Internal::Type <S>::ConstRef val)
{
	if (_swtich (__d) != 3) {
		_destruct ();
		::CORBA::Internal::_construct (_u._w, val);__d = 3;
	} else_u._w = val;}
::CORBA::Internal::Type <I1>::Var& obj ()
{
	if (_swtich (__d) != 5)
		throw ::CORBA::BAD_PARAM ();
	return _u._obj;
}
void obj (::CORBA::Internal::Type <I1>::ConstRef val)
{
	if (_swtich (__d) != 5) {
		_destruct ();
		::CORBA::Internal::_construct (_u._obj, val);__d = 5;
	} else_u._obj = val;}
void obj (::CORBA::Internal::Type <I1>::Var&& val)
{
	if (_swtich (__d) != 5) {
		_destruct ();
		::CORBA::Internal::_construct (_u._obj, std::move (val));__d = 5;
	} else_u._obj = std::move (val);}

void U::_destruct () NIRVANA_NOEXCEPT
{
	switch (__d) {
		case 2:
			::CORBA::Internal::destruct (_u._z);
			break;
		default:
			::CORBA::Internal::destruct (_u._obj);
	}
	__d = 5;
	::CORBA::Internal::construct (_u._obj);
}

::CORBA::Long U::_switch (::CORBA::Long d) NIRVANA_NOEXCEPT
{
	switch (d) {
	case 1:
		return 1;
	case 2:
		return 2;
	case 3:
	case 4:
		return 3;
	default:
		return 5;
	}
}

void UFixed::_d (::CORBA::Long d)
{
	if (_switch (d) != _switch (__d))
		throw ::CORBA::BAD_PARAM ();
}

::CORBA::Long& x ()
{
	if (_swtich (__d) != 1)
		throw ::CORBA::BAD_PARAM ();
	return _u._x;
}
void x (::CORBA::Internal::Type < ::CORBA::Long>::ConstRef val)
{
	if (_swtich (__d) != 1) {
		_destruct ();
		::CORBA::Internal::_construct (_u._x, val);__d = 1;
	} else_u._x = val;}
::CORBA::Double& d ()
{
	if (_swtich (__d) != 2)
		throw ::CORBA::BAD_PARAM ();
	return _u._d;
}
void d (::CORBA::Internal::Type < ::CORBA::Double>::ConstRef val)
{
	if (_swtich (__d) != 2) {
		_destruct ();
		::CORBA::Internal::_construct (_u._d, val);__d = 2;
	} else_u._d = val;}

::CORBA::Long UFixed::_switch (::CORBA::Long d) NIRVANA_NOEXCEPT
{
	switch (d) {
	case 1:
		return 1;
	case 2:
		return 2;
	default:
		return 3;
	}
}

void Z::_d (::CORBA::Boolean d)
{
	if (_switch (d) != _switch (__d))
		throw ::CORBA::BAD_PARAM ();
}

::CORBA::Short& s ()
{
	if (_swtich (__d) != true)
		throw ::CORBA::BAD_PARAM ();
	return _u._s;
}
void s (::CORBA::Internal::Type < ::CORBA::Short>::ConstRef val)
{
	if (_swtich (__d) != true) {
		_destruct ();
		::CORBA::Internal::_construct (_u._s, val);__d = true;
	} else_u._s = val;}

}

NIRVANA_OLF_SECTION_N (1) extern const Nirvana::ImportInterfaceT <CORBA::TypeCode>
Test::_tc_Tag = { Nirvana::OLF_IMPORT_INTERFACE, CORBA::Internal::RepIdOf <Test::Tag>::id, CORBA::Internal::RepIdOf <CORBA::TypeCode>::id };

namespace Test {

void Tagged::_d (Tag d)
{
	if (_switch (d) != _switch (__d))
		throw ::CORBA::BAD_PARAM ();
}

::CORBA::Short& s ()
{
	if (_swtich (__d) != Tag::Second)
		throw ::CORBA::BAD_PARAM ();
	return _u._s;
}
void s (::CORBA::Internal::Type < ::CORBA::Short>::ConstRef val)
{
	if (_swtich (__d) != Tag::Second) {
		_destruct ();
		::CORBA::Internal::_construct (_u._s, val);__d = Tag::Second;
	} else_u._s = val;}

Tag Tagged::_switch (Tag d) NIRVANA_NOEXCEPT
{
	switch (d) {
	case Tag::Second:
		return Tag::Second;
	default:
		return Tag::First;
	}
}

}
