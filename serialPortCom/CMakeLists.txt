add_library( serialPortComLib
    ascii/FlightMetaInfo.h
    ascii/FlightMetaInfo.cpp
    ascii/NmeaSentence.h
    ascii/NmeaSentence.cpp
    ascii/StringUtils.h
    ascii/StringUtils.cpp
    binary/BinaryBlock.h
    binary/BinaryBlock.cpp
    binary/Convert.h
    binary/Convert.cpp
    binary/FlightInformationRecord.h
    binary/FlightInformationRecord.cpp
    binary/KeyTrackPositionRecord.h
    binary/KeyTrackPositionRecord.cpp
    binary/TrackPositionRecordDeltas.h
    binary/TrackPositionRecordDeltas.cpp
    IgcFactory.h
    IgcFactory.cpp
    IgcFile.h
    IgcFile.cpp
    Request.h
    Request.cpp
    Request.test.cpp
    SerialPort.h
    SerialPort.cpp
    )
target_compile_definitions( serialPortComLib PRIVATE BOOST_ALL_DYN_LINK _WIN32_WINNT=0x0501)

add_executable( serialPortComLibTest
    ascii/NmeaSentence.test.cpp
    ascii/FlightMetaInfo.test.cpp
    ascii/StringUtils.test.cpp
    binary/BinaryBlock.test.cpp
    binary/Convert.test.cpp
    )
target_link_libraries( serialPortComLibTest
    progressLib
    serialPortComLib
    gtest
    gtest_main
)
target_compile_definitions( serialPortComLibTest PRIVATE BOOST_ALL_DYN_LINK _WIN32_WINNT=0x0501)
add_test(NAME serialPortComLibTest COMMAND ${CMAKE_BINARY_DIR}/serialPortComLib/serialPortComLibTest )
