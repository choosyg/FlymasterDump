#include "BinaryBlock.h"
#include "Convert.h"
#include <gtest/gtest.h>

TEST(BinaryBlockTest, NoPacketId){
    std::string bytes;
    ASSERT_ANY_THROW( BinaryBlock{ bytes } );
}

TEST(BinaryBlockTest, InvalidPacketId){
    std::string bytes = toBytes(0xa4a4);
    ASSERT_ANY_THROW( BinaryBlock{ bytes } );
}

TEST(BinaryBlockTest, EndOfData){
    std::string bytes = toBytes(PacketId::END_OF_DATA);
    BinaryBlock block( bytes );
    ASSERT_EQ( PacketId::END_OF_DATA, block.packetId );
}

TEST(BinaryBlockTest, NoLenght){
    std::string bytes = toBytes(PacketId::FLIGHT_INFORMAION_RECORD);
    ASSERT_ANY_THROW( BinaryBlock{ bytes } );
}

TEST(BinaryBlockTest, InvalidLenght){
    std::string bytes = toBytes(PacketId::FLIGHT_INFORMAION_RECORD);
    bytes += toBytes( 10 );
    ASSERT_ANY_THROW( BinaryBlock{ bytes } );
}

TEST(BinaryBlockTest, InvalidCrc){
    std::string bytes = toBytes(PacketId::FLIGHT_INFORMAION_RECORD);
    bytes += toBytes( 10 );
    for( size_t i = 0; i<10; ++i ){
        bytes += toBytes( 0 );
    }
    bytes += toBytes( 1 );
    ASSERT_ANY_THROW( BinaryBlock{ bytes } );
}

TEST(BinaryBlockTest, Perfect){
    std::string bytes = toBytes(PacketId::FLIGHT_INFORMAION_RECORD);
    bytes += toBytes( 10 );
    for( size_t i = 0; i<10; ++i ){
        bytes += toBytes( 0 );
    }
    bytes += toBytes( 0 );
    ASSERT_ANY_THROW( BinaryBlock{ bytes } );
}
