#ifndef NIRVANA_TESTORB_I3_FACTORY_TIED_H_
#define NIRVANA_TESTORB_I3_FACTORY_TIED_H_

#include <Nirvana/static.h>
#include <CORBA/CORBA.h>
#include "Test_I3_factory.h"

NIRVANA_STATIC ((Test, I3_factory_tied), ::Test::I3_factory)
NIRVANA_STATIC ((Test, I3_tied_derived), ::Test::I3_factory)

#endif
