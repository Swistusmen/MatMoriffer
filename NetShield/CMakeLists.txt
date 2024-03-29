cmake_minimum_required(VERSION 3.14)

project(NetShield VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core Quick Charts REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Charts REQUIRED)


set(PROJECT_SOURCES
        main.cpp
        Analyzer.h
        Analyzer.cpp
        DriverCommunication.cpp
        DriverCommunication.h
        InterMessageBroker.cpp
        InterMessageBroker.h
        JsonReader.h
        JsonReader.cpp
        Logger.h
        Logger.cpp
        DriverSocket.h
        DriverSocket.cpp
        Target.h
        Common.h
        qml.qrc
        ../kernel/lib/libmatmoriffer.h
        ../kernel/lib/libmatmoriffer.c
        ${TS_FILES}
)


add_executable(NetShield
  ${PROJECT_SOURCES}
)


target_compile_definitions(NetShield
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(NetShield
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Charts)

set_target_properties(NetShield PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)
