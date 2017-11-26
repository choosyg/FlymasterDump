
#############################################################
# QT
#############################################################
set( CMAKE_PREFIX_PATH "C:/Qt/5.9.2/msvc2017_64" )
find_package(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})
find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS})

get_filename_component(QT_DIR ${QT_QMAKE_EXECUTABLE} DIRECTORY)
install( FILES ${QT_DIR}/Qt5Core.dll
               ${QT_DIR}/Qt5Widgets.dll
               ${QT_DIR}/Qt5Gui.dll DESTINATION . )
install( FILES ${QT_DIR}/../plugins/platforms/qwindows.dll DESTINATION ./platforms )
file( COPY ${QT_DIR}/Qt5Core.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${QT_DIR}/Qt5Widgets.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${QT_DIR}/Qt5Gui.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${QT_DIR}/Qt5Cored.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${QT_DIR}/Qt5Widgetsd.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${QT_DIR}/Qt5Guid.dll DESTINATION ${PROJECT_BINARY_DIR}/app )

#############################################################
# BOOST
#############################################################
set( BOOST_DIR "${PROJECT_SOURCE_DIR}/../boost-1.65.1" )
include_directories( ${BOOST_DIR} )
link_directories( "${BOOST_DIR}/stage/lib" )
file( COPY ${BOOST_DIR}/stage/lib/boost_system-vc141-mt-1_65_1.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${BOOST_DIR}/stage/lib/boost_date_time-vc141-mt-1_65_1.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${BOOST_DIR}/stage/lib/boost_system-vc141-mt-gd-1_65_1.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${BOOST_DIR}/stage/lib/boost_date_time-vc141-mt-gd-1_65_1.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
install( FILES
    ${BOOST_DIR}/stage/lib/boost_system-vc141-mt-1_65_1.dll
    ${BOOST_DIR}/stage/lib/boost_date_time-vc141-mt-1_65_1.dll
    DESTINATION . )

#############################################################
# GTest
#############################################################
set( GTEST_DIR "${PROJECT_SOURCE_DIR}/../gtest-1.7.0" )
include_directories( ${GTEST_DIR}/include )
link_directories( "${GTEST_DIR}/lib" )
set( GTEST_LIBS
    optimized gtest
    debug gtestD
    optimized gtest_main
    debug gtest_mainD
    )
