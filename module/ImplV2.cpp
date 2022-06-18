#include <CORBA/Server.h>
#include <IDL/Test_V2_s.h>

namespace Test {

class ImplV2 : public CORBA::servant_traits <V2>::Servant <ImplV2>
{
public:
	ImplV2 ()
	{}

	~ImplV2 ()
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

	int32_t aop (int32_t x)
	{
		return x;
	}
};

}

NIRVANA_VALUETYPE_IMPL (Test::V2, Test::ImplV2)
