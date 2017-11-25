#include <gtest/gtest.h>
#include "StringUtils.h"

TEST( StringUtilsTest, splitEmptyPart ) {
    std::string str = "0,,1";
    auto list = split( str, ',' );
    ASSERT_EQ( 3, list.size() );
    ASSERT_EQ( "0", list[0] );
    ASSERT_TRUE( list[1].empty() );
    ASSERT_EQ( "1", list[2] );
}

TEST( StringUtilsTest, splitEmptyString ) {
    std::string str = "";
    auto list = split( str, ',' );
    ASSERT_TRUE( list.empty() );
}

TEST( StringUtilsTest, splitDelAtEnd ) {
    std::string str = "1,";
    auto list = split( str, ',' );
    ASSERT_EQ( 2, list.size() );
    ASSERT_EQ( "1", list[0] );
    ASSERT_TRUE( list[1].empty() );
}

TEST( StringUtilsTest, splitBasic ) {
    std::string str = "1,2";
    auto list = split( str, ',' );
    ASSERT_EQ( 2, list.size() );
    ASSERT_EQ( "1", list[0] );
    ASSERT_EQ( "2", list[1] );
}

TEST( StringUtilsTest, splitNoDelim ) {
    std::string str = "1";
    auto list = split( str, ',' );
    ASSERT_EQ( 1, list.size() );
    ASSERT_EQ( "1", list[0] );
}

TEST( StringUtilsTest, join ) {
    StringList list;
    list.push_back( "1" );
    list.push_back( "" );
    list.push_back( "2" );
    ASSERT_EQ( "1,,2", join( list, ',' ) );
}

TEST( StringUtilsTest, joinEmpty ) {
    StringList list;
    ASSERT_EQ( "", join( list, ',' ) );
}
