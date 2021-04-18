#ifndef IDL_TEST_I2_S_H_
#define IDL_TEST_I2_S_H_

#include "Test_I2.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Test::I2>
{
public:
	static const typename Bridge < ::Test::I2>::EPV epv_;

protected:
	static Long _op2 (Bridge < ::Test::I2>* _b, Long p1, Interface* _env)
	{
		try {
			return S::_implementation (_b).op2 (p1);
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}
};

template <class S>
const Bridge < ::Test::I2>::EPV Skeleton <S, ::Test::I2>::epv_ = {
	{ // header
		Bridge < ::Test::I2>::repository_id_,
		S::template __duplicate < ::Test::I2>,
		S::template __release < ::Test::I2>
	},
	{ // base
		S::template _wide_object <::Test::I2>
	},
	{ // epv
		S::_op2
	}
};

// Standard implementation

template <class S>
class Servant <S, ::Test::I2> :
	public Implementation <S, ::Test::I2>
{};

}
}

// POA implementation
#ifndef TEST_NO_POA
namespace CORBA {
namespace Nirvana {

template <>
class ServantPOA < ::Test::I2> :
	public ImplementationPOA < ::Test::I2>
{
public:
	virtual Long op2 (Long p1) = 0;
};

}
}

namespace POA_Test {
typedef ::CORBA::Nirvana::ServantPOA < ::Test::I2> I2;
}
#endif

// Static implementation
#ifndef TEST_NO_STATIC

namespace CORBA {
namespace Nirvana {

template <class S>
class ServantStatic <S, ::Test::I2> :
	public ImplementationStatic <S, ::Test::I2>
{};

}
}

#endif

// Tied implementation
#ifndef TEST_NO_TIED

namespace POA_Test {

template <class T> using I2_tie = ::CORBA::Nirvana::ServantTied <T, ::Test::I2>;

}

#endif

#endif
