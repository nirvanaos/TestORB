#include <CORBA/Server.h>
#include <IDL/Test_V1_s.h>

namespace Test {

class ImplV1 : public CORBA::servant_traits <V1>::Servant <ImplV1>
{
public:
	ImplV1 ()
	{}

	ImplV1 (int16_t v1, std::string v3)
	{
		val1 (v1);
		val3 (std::move (v3));
	}

	~ImplV1 ()
	{}

	int16_t op1 ()
	{
		return 0;
	}

	int32_t op2 (V1::_ptr_type pv)
	{
		return pv->val2 ();
	}
};

}

NIRVANA_VALUETYPE_IMPL (Test::V1, Test::ImplV1)
