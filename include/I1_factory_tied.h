#ifndef NIRVANA_TESTORB_I1_FACTORY_TIED_H_
#define NIRVANA_TESTORB_I1_FACTORY_TIED_H_
#pragma once

#include <CORBA/CORBA.h>
#include <IDL/Test_I1_factory.h>
#include <Nirvana/static.h>

NIRVANA_STATIC ((Test, I1_factory_tied), ::Test::I1_factory)
NIRVANA_STATIC ((Test, I1_tied_derived), ::Test::I1_factory)

#endif
