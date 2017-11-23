find_package(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})
find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
#find_package(Qt5Gui REQUIRED)
#include_directories(${Qt5Gui_INCLUDE_DIRS})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set( UIS
    MainWindow.ui
)

set_source_files_properties(${UIS} PROPERTIES HEADER_FILE_ONLY TRUE)

add_executable( variodump
    DumpTask.h
    DumpTask.cpp
    FlightListTask.h
    FlightListTask.cpp
    main.cpp
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
set_target_properties(variodump PROPERTIES WIN32_EXECUTABLE YES LINK_FLAGS "/ENTRY:mainCRTStartup")
target_include_directories(variodump PRIVATE
    "../serialPortComLib"
    )
target_link_libraries( variodump
    progressLib
    serialPortComLib
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
)
target_compile_definitions( variodump PRIVATE BOOST_ALL_DYN_LINK _WIN32_WINNT=0x0501)

install(TARGETS variodump RUNTIME DESTINATION .)

get_filename_component(QT_DIR ${QT_QMAKE_EXECUTABLE} DIRECTORY)
install( FILES ${QT_DIR}/Qt5Core.dll
               ${QT_DIR}/Qt5Widgets.dll
               ${QT_DIR}/Qt5Gui.dll DESTINATION . )
install( FILES ${QT_DIR}/../plugins/platforms/qwindows.dll DESTINATION ./platforms )
