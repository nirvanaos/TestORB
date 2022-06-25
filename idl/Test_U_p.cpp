// This file was generated from "Test_U.idl"
// Nirvana IDL compiler version 1.0
#include <CORBA/Proxy/Proxy.h>
#include "Test_U_s.h"

namespace CORBA {
namespace Internal {

template <>
const Char TypeCodeName < ::Test::S>::name_ [] = "S";
template <>
const Parameter TypeCodeMembers < ::Test::S>::members_ [] = {
	{ "len", Type <Long>::type_code }
};


void Type < ::Test::S>::byteswap (Var& v) NIRVANA_NOEXCEPT
{
	Type <Long>::byteswap (v._len);
}

}
}
NIRVANA_EXPORT (_exp_Test_S, CORBA::Internal::RepIdOf <Test::S>::id, CORBA::TypeCode, CORBA::Internal::TypeCodeStruct <Test::S>)

namespace CORBA {
namespace Internal {

template <>
const Char TypeCodeName < ::Test::Tag>::name_ [] = "Tag";

template <>
const char* const TypeCodeEnum < ::Test::Tag>::members_ [] = {
	"First",
	"Second"
};

}
}
NIRVANA_EXPORT (_exp_Test_Tag, CORBA::Internal::RepIdOf <Test::Tag>::id, CORBA::TypeCode, CORBA::Internal::TypeCodeEnum <Test::Tag>)
