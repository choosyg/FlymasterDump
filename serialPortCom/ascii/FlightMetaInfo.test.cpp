#include <gtest/gtest.h>
#include "FlightMetaInfo.h"

TEST( FlightMetaInfoTest, shouldFailIfWrongCommand ) {
    NmeaSentence s( "$PFMSNP,*3A" );
    ASSERT_ANY_THROW( FlightMetaInfo k( s ) );
}

TEST( FlightMetaInfoTest, validSentence ) {
    NmeaSentence s( "$PFMLST,056,001,06.10.17,07:38:05,00:05:48*3F" );
    FlightMetaInfo k( s );
    ASSERT_EQ( "$PFMDNL,171006073805,*15", k.flightRequest() );
}
