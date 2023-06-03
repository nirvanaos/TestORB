#ifndef NIRVANA_TESTORB_I2_FACTORY_V3_H_
#define NIRVANA_TESTORB_I2_FACTORY_V3_H_
#pragma once

#include <CORBA/CORBA.h>
#include <IDL/Test_I2.h>
#include <Nirvana/static.h>

// Create V3 valuetype and return I2 interface
NIRVANA_STATIC ((Test, I2_factory_V3), ::Test::I2_factory)

#endif
