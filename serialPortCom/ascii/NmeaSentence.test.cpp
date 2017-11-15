#include <gtest/gtest.h>
#include "NmeaSentence.h"

TEST( NmeaSentenceTest, shouldFailIfWrongSyntax ) {
    ASSERT_ANY_THROW( NmeaSentence( "PFMSNP,*3A" ) );
}

TEST( NmeaSentenceTest, shouldParse ) {
    ASSERT_EQ( "$PFMSNP,*3A", NmeaSentence( "$PFMSNP,*3A" ).build() );
    ASSERT_EQ( "$PFMDNL,LST,*56", NmeaSentence( "$PFMDNL,LST,*56" ).build() );
    ASSERT_EQ( "$PFMSNP,GpsSD,,00857,1.06p,1019.87,*3E",
               NmeaSentence( "$PFMSNP,GpsSD,,00857,1.06p,1019.87,*3E" ).build() );
}
