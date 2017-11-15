#pragma once

#include "SerialPort.h"
#include "ascii/FlightMetaInfo.h"
#include "binary/BinaryBlock.h"

#include <progress/Callback.h>

std::string requestSerial( SerialPort& port, Callback& cb );

std::vector< FlightMetaInfo > requestFlightInfos( SerialPort& port, Callback& cb );

BinaryFlightData requestFlight( SerialPort& port, FlightMetaInfo& info, Callback& cb );
