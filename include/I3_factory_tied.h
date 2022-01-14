#ifndef NIRVANA_TESTORB_I3_FACTORY_TIED_H_
#define NIRVANA_TESTORB_I3_FACTORY_TIED_H_
#pragma once

#include <CORBA/CORBA.h>
#include <IDL/Test_I3_factory.h>
#include <Nirvana/static.h>

NIRVANA_STATIC ((Test, I3_factory_tied), ::Test::I3_factory)
NIRVANA_STATIC ((Test, I3_tied_derived), ::Test::I3_factory)

#endif
