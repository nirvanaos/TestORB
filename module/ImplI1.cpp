#include "ImplI1.h"

namespace Test {

using namespace CORBA;

I1::_ref_type ImplI1::object_op (I1::_ptr_type in_obj, I1::_ref_type& out_obj, I1::_ref_type& inout_obj)
{
	I1::_ref_type tmp (std::move (inout_obj));
	out_obj = I1::_duplicate (in_obj);
	inout_obj = I1::_duplicate (in_obj);
	return tmp;
}

std::string ImplI1::string_op (const std::string& in_s, std::string& out_s, std::string& inout_s)
{
	std::string tmp (std::move (inout_s));
	out_s = in_s;
	inout_s = in_s;
	return tmp;
}

std::vector <Long> ImplI1::seq_op (const std::vector <Long>& in_s, std::vector <Long>& out_s, std::vector <Long>& inout_s)
{
	std::vector <Long> tmp (std::move (inout_s));
	out_s = in_s;
	inout_s = in_s;
	return tmp;
}

CORBA::Any ImplI1::any_op (const CORBA::Any& in_any, CORBA::Any& out_any, CORBA::Any& inout_any)
{
	CORBA::Any tmp (std::move (inout_any));
	out_any = in_any;
	inout_any = in_any;
	return tmp;
}

}