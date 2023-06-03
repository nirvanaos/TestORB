#include <Nirvana/Nirvana.h>
#include "ImplI2.h"

using namespace CORBA;

namespace Test {

int32_t ImplI2::divide (int32_t a, int32_t b)
{
	return a / b;
}

#ifdef LEGACY_CORBA_CPP

A1::_ptr_type ImplI2::abstract_op (A1::_ptr_type in_obj, A1::_var_type& out_obj, A1::_var_type& inout_obj)
{
	A1::_var_type tmp (std::move (inout_obj));
	out_obj = A1::_duplicate (in_obj);
	inout_obj = A1::_duplicate (in_obj);
	return tmp._retn ();
}

#else

A1::_ref_type ImplI2::abstract_op (A1::_ptr_type in_obj, A1::_ref_type& out_obj, A1::_ref_type& inout_obj)
{
	A1::_ref_type tmp (std::move (inout_obj));
	out_obj = in_obj;
	inout_obj = in_obj;
	return tmp;
}

#endif

#ifdef LEGACY_CORBA_CPP

V1::_ptr_type ImplI2::value_op (V1::_ptr_type in_obj, V1::_var_type& out_obj, V1::_var_type& inout_obj)
{
	V1::_var_type tmp = V1::_duplicate (V1::_downcast (inout_obj->_copy_value ()));
	out_obj = V1::_duplicate (in_obj);
	inout_obj = V1::_duplicate (in_obj);
	return tmp._retn ();
}

#else

V1::_ref_type ImplI2::value_op (V1::_ptr_type in_obj, V1::_ref_type& out_obj, V1::_ref_type& inout_obj)
{
	V1::_ref_type tmp = V1::_downcast (inout_obj->_copy_value ());
	inout_obj = nullptr;
	out_obj = in_obj;
	inout_obj = in_obj;
	return tmp;
}

#endif

#ifdef LEGACY_CORBA_CPP

StringValue::_ptr_type ImplI2::box_op (StringValue::_ptr_type in_obj, StringValue::_var_type& out_obj, StringValue::_var_type& inout_obj)
{
	StringValue::_var_type tmp (std::move (inout_obj));
	out_obj = StringValue::_duplicate (in_obj);
	inout_obj = StringValue::_duplicate (in_obj);
	return tmp._retn ();
}

#else

StringValue::_ref_type ImplI2::box_op (StringValue::_ptr_type in_obj, StringValue::_ref_type& out_obj, StringValue::_ref_type& inout_obj)
{
	StringValue::_ref_type tmp (std::move (inout_obj));
	out_obj = in_obj;
	inout_obj = in_obj;
	return tmp;
}

#endif

BoxArray ImplI2::box_array_op (const BoxArray& in_ar, BoxArray& out_ar, BoxArray& inout_ar)
{
	BoxArray tmp (std::move (inout_ar));
	out_ar = in_ar;
	inout_ar = in_ar;
	return tmp;
}

U ImplI2::union_op (const U& in_u, U& out_u, U& inout_u)
{
	U tmp (inout_u);
	out_u = in_u;
	inout_u = in_u;
	return tmp;
}

#ifdef LEGACY_CORBA_CPP

TypeCode::_ptr_type ImplI2::type_code_op (TypeCode::_ptr_type in_obj, TypeCode::_var_type& out_obj, TypeCode::_var_type& inout_obj)
{
	TypeCode::_var_type tmp (std::move (inout_obj));
	out_obj = TypeCode::_duplicate (in_obj);
	inout_obj = TypeCode::_duplicate (in_obj);
	return tmp._retn ();
}

#else

TypeCode::_ref_type ImplI2::type_code_op (TypeCode::_ptr_type in_obj, TypeCode::_ref_type& out_obj, TypeCode::_ref_type& inout_obj)
{
	TypeCode::_ref_type tmp (std::move (inout_obj));
	out_obj = in_obj;
	inout_obj = in_obj;
	return tmp;
}

#endif

}
