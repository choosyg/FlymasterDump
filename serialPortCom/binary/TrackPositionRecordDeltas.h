#pragma once

#include "BinaryBlock.h"

class TrackPositionRecordDeltas {
public:
    TrackPositionRecordDeltas( const BinaryFlightData& bfd );

    struct Delta {
        uint8_t fixFlag;
        int8_t latitude;
        int8_t longitude;
        int8_t altitude;
        int8_t pressure;
        uint8_t time;
    };
    std::vector< Delta > deltas;
};
