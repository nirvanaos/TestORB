// This file will be generated by the IDL compiler in the future.
#ifndef IDL_TEST_I3_FACTORY_S_H_
#define IDL_TEST_I3_FACTORY_S_H_

#include "Test_I3_factory.h"
#include <CORBA/Server.h>

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Test::I3_factory>
{
public:
	static const typename Bridge < ::Test::I3_factory>::EPV epv_;

protected:
	static Interface* _create (Bridge < ::Test::I3_factory>* _b, Long addendum, Interface* _env)
	{
		try {
			return TypeI < ::Test::I3>::ret (S::_implementation (_b).create (addendum));
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}
};

template <class S>
const Bridge < ::Test::I3_factory>::EPV Skeleton <S, ::Test::I3_factory>::epv_ = {
	{ // header
		Bridge < ::Test::I3_factory>::repository_id_,
		S::template __duplicate < ::Test::I3_factory>,
		S::template __release < ::Test::I3_factory>
	},
	{ // base
		S::template _wide_object <::Test::I3_factory>
	},
	{ // epv
		S::_create
	}
};

// Static implementation
template <class S>
class ServantStatic <S, ::Test::I3_factory> :
	public ImplementationLocalStatic <S, ::Test::I3_factory>
{};

}
}

#endif
