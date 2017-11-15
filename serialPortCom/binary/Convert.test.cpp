#include "Convert.h"

#include <gtest/gtest.h>

TEST( ConvertTest, bytesToInt8 ) {
    std::string str;
    str += (char)0xb1;
    ASSERT_EQ( (int8_t)0xb1, fromBytes<int8_t>( str.c_str() ) );
}

TEST( ConvertTest, bytesToUint8 ) {
    std::string str;
    str += (unsigned char)0xb1;
    ASSERT_EQ( (uint8_t)0xb1, fromBytes<uint8_t>( str.c_str() ) );
}

TEST( ConvertTest, bytesToInt16 ) {
    std::string str;
    str += (char)0xb1;
    str += (char)0xb2;
    ASSERT_EQ( (int16_t)0xb2b1, fromBytes<int16_t>( str.c_str() ) );
}

TEST( ConvertTest, bytesToUint16 ) {
    std::string str;
    str += (char)0xb1;
    str += (char)0xb2;
    ASSERT_EQ( (uint16_t)0xb2b1, fromBytes<uint16_t>( str.c_str() ) );
}

TEST( ConvertTest, bytesToInt32 ) {
    std::string str;
    str += (char)0xb1;
    str += (char)0xb2;
    str += (char)0xb3;
    str += (char)0xb4;
    ASSERT_EQ( (int32_t)0xb4b3b2b1, fromBytes<int32_t>( str.c_str() ) );
}

TEST( ConvertTest, bytesToUint32 ) {
    std::string str;
    str += (char)0xb1;
    str += (char)0xb2;
    str += (char)0xb3;
    str += (char)0xb4;
    ASSERT_EQ( (uint32_t)0xb4b3b2b1, fromBytes<uint32_t>( str.c_str() ) );
}
