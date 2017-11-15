#pragma once

#include "IgcFile.h"
#include "ascii/FlightMetaInfo.h"
#include "binary/BinaryBlock.h"

IgcFile buildIgc( const FlightMetaInfo& fmi, const BinaryFlightData& bfd );
