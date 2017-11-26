set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set( UIS
    MainWindow.ui
)
set_source_files_properties(${UIS} PROPERTIES HEADER_FILE_ONLY TRUE)

add_library( varioDumpUiLib
    DumpTask.h
    DumpTask.cpp
    FlightListTask.h
    FlightListTask.cpp
    MainWindow.h
    MainWindow.cpp
    Model.h
    Model.cpp
    Property.h
    QtCallback.h
    QtCallback.cpp
    TaskProcessor.h
    TaskProcessor.cpp
    TaskProgressDialog.h
    TaskProgressDialog.cpp
    ${UIS}
    )
target_compile_definitions( varioDumpUiLib PRIVATE BOOST_ALL_DYN_LINK _WIN32_WINNT=0x0501)

add_executable( variodump main.cpp )
target_link_libraries( variodump
    varioDumpUiLib
    progressLib
    serialPortComLib
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
)
set_target_properties(variodump PROPERTIES WIN32_EXECUTABLE YES LINK_FLAGS "/ENTRY:mainCRTStartup")
target_compile_definitions( variodump PRIVATE BOOST_ALL_DYN_LINK _WIN32_WINNT=0x0501)
install(TARGETS variodump RUNTIME DESTINATION .)

add_executable( variodumpTest
    TaskProcessor.test.cpp
    )
target_link_libraries( variodumpTest
    varioDumpUiLib
    progressLib
    serialPortComLib
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    ${GTEST_LIBS}
)
target_compile_definitions( variodumpTest PRIVATE BOOST_ALL_DYN_LINK _WIN32_WINNT=0x0501)
add_test(NAME variodumpTest COMMAND ${CMAKE_BINARY_DIR}/app/variodumpTest )


