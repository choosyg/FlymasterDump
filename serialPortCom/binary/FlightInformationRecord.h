#pragma once

#include "BinaryBlock.h"

class FlightInformationRecord {
public:
    FlightInformationRecord( const BinaryFlightData& bfd );
    int16_t firmwareVersion;
    int16_t hardwareVersion;
    int32_t deviceSerial;
    std::string pilotCompetitionNumber;
    std::string pilotName;
    std::string gliderBrand;
    std::string gliderModel;
};
