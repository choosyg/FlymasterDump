cmake_minimum_required (VERSION 3.9.0)
project( VarioDump )

set( CMAKE_PREFIX_PATH "C:/Qt/5.9.2/msvc2017_64" )

set( VarioDump_VERSION_MAJOR 1 )
set( VarioDump_VERSION_MINOR 0 )
set( VarioDump_VERSION_PATCH 0 )

set( BOOST_DIR "${PROJECT_SOURCE_DIR}/../boost-1.65.1" )
include_directories( ${BOOST_DIR} )
link_directories( "${BOOST_DIR}/stage/lib" )
file( COPY ${BOOST_DIR}/stage/lib/boost_system-vc141-mt-1_65_1.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${BOOST_DIR}/stage/lib/boost_date_time-vc141-mt-1_65_1.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${BOOST_DIR}/stage/lib/boost_regex-vc141-mt-1_65_1.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${BOOST_DIR}/stage/lib/boost_system-vc141-mt-gd-1_65_1.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${BOOST_DIR}/stage/lib/boost_date_time-vc141-mt-gd-1_65_1.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${BOOST_DIR}/stage/lib/boost_regex-vc141-mt-gd-1_65_1.dll DESTINATION ${PROJECT_BINARY_DIR}/app )

install( FILES
    ${BOOST_DIR}/stage/lib/boost_system-vc141-mt-1_65_1.dll
    ${BOOST_DIR}/stage/lib/boost_date_time-vc141-mt-1_65_1.dll
    ${BOOST_DIR}/stage/lib/boost_regex-vc141-mt-1_65_1.dll
    DESTINATION . )

set( GTEST_DIR "${PROJECT_SOURCE_DIR}/../gtest-1.7.0" )
include_directories( ${GTEST_DIR}/include )
link_directories( "${GTEST_DIR}/lib" )
set( GTEST_LIBS
    optimized gtest
    debug gtestD
    optimized gtest_main
    debug gtest_mainD
    )

# CTest
include(CTest)
enable_testing()

include_directories("${VarioDump_SOURCE_DIR}")

add_subdirectory( progress )
add_subdirectory( serialPortCom )
add_subdirectory( app )

set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_VENDOR "Schiffler")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "It's great")
set(CPACK_PACKAGE_VERSION ${VarioDump_VERSION_MAJOR}.${VarioDump_VERSION_MINOR}.${VarioDump_VERSION_PATCH}-${CMAKE_BUILD_TYPE})
set(CPACK_PACKAGE_VERSION_MAJOR ${VarioDump_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VarioDump_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VarioDump_VERSION_PATCH})
include(CPack)
