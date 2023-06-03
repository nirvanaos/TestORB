#include <CORBA/Server.h>
#include <IDL/Test_V3_s.h>
#include <I2_factory_V3.h>
#include "ImplI2.h"

using namespace CORBA;

namespace Test {

class ImplV3 : public CORBA::servant_traits <V3>::Servant <ImplV3>,
	public ImplI2
{
public:
	ImplV3 ()
	{}

	ImplV3 (int32_t a)
	{
		addendum (a);
	}

	~ImplV3 ()
	{}

	int16_t vop1 ()
	{
		return 0;
	}

	int32_t vop2 (V1::_ptr_type pv)
	{
		return pv->val2 ();
	}

	int32_t op_v2 (int32_t x)
	{
		val_v2 (x);
		return x;
	}

	int32_t op2 (int32_t p1)
	{
		return p1 + 2 * addendum ();
	}

	int32_t aop (int32_t x)
	{
		return x;
	}
};

class I2_factory_V3 :
	public servant_traits <I2_factory>::ServantStatic <I2_factory_V3>
{
public:
	static
#ifdef LEGACY_CORBA_CPP
		I2::_var_type
#else
		I2::_ref_type
#endif
		create (int32_t addendum)
	{
		V3::_ref_type v = V3_factory::_factory->create (addendum);

		// Supported interface operations must be available
		v->op2 (0);
		v->aop (0);

		// Conversion to supported interface ptr must be unavailable:
		// I2::_ptr_type p = v; // compiler error

		// Direct conversion to ValueBase must be available
		ValueBase::_ptr_type vb = v;

		// Return I2 proxy.
		return v->_this ();
	}
};

}

NIRVANA_VALUETYPE_IMPL (Test::V3, Test::ImplV3)
NIRVANA_STATIC_EXP (Test, I2_factory_V3)
