#pragma once

#include "BinaryBlock.h"

class KeyTrackPositionRecord {
public:
    KeyTrackPositionRecord( const BinaryFlightData& bfd );
    uint8_t fixFlag;   // 8th bit==1 is 'A' otherwise 'V'
    int32_t latiude;   // <0 South, >0 North
    int32_t longitude; // <0 East, >0 West
    int16_t altiude;
    int16_t pressure;
    uint32_t time;
};
