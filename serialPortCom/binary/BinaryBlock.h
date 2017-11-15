#pragma once

#include <string>
#include <vector>

enum class PacketId : uint16_t {
    FLIGHT_INFORMAION_RECORD = 0xa0a0,
    KEY_TRACK_POSITION_RECORD = 0xa1a1,
    TRACK_POSITION_RECORD_DELTA = 0xa2a2,
    END_OF_DATA = 0xa3a3
};

class BinaryBlock {
public:
    BinaryBlock( const std::string& bytes );
    PacketId packetId = PacketId::END_OF_DATA;
    uint8_t length = 0;
    std::string data;

private:
    char crc( const std::string& str ) const;
};
typedef std::vector< BinaryBlock > BinaryFlightData;
