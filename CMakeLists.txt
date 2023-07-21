cmake_minimum_required(VERSION 3.12)

project(DataReport VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Quick)


include("3rdparty/FluentUI/FluentUI.cmake")
include("3rdparty/QXlsx/QXlsx.cmake")
include("3rdparty/SingleApplication/SingleApplication.cmake")

if (MOROSE_TEST)
    add_compile_definitions("MOROSE_TEST")
endif(MOROSE_TEST)

set(
    PROJECT_SOURCES
    main.cpp
    morose_config.h
    morose.qrc
    common/common.h
    common/common.cpp
    icon.rc
    common/views/qwaveview.cpp
    common/views/qwaveview.h
    common/views/datatablemodel.h
    common/views/datatablemodel.cpp
    common/ruitiedefine.h
    common/ruitie.h
    common/ruitie.cpp
    common/msg/discoveryrecordmsg.h
    common/msg/discoveryrecordmsg.cpp
    common/msg/inspectionrecordmsg.h
    common/msg/inspectionrecordmsg.cpp
    common/msg/quarterlyrecordmsg.h
    common/msg/quarterlyrecordmsg.cpp
    common/msg/dailyrecordmsg.h
    common/msg/dailyrecordmsg.cpp
    common/views/defectrecordview.h
    common/views/defectrecordview.cpp
    common/views/inspectionrecordview.h
    common/views/inspectionrecordview.cpp
    common/views/performancerecordview.h
    common/views/performancerecordview.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Quick QXlsx::QXlsx SingleApplication::SingleApplication)

set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Morose)

set(MOROSE_DIST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../x64/Debug/DataReport")

if (Morose_FOUND)
    morose_main_setup()
    # 添加qml_dirs用于windeployqt部署
    morose_add_qml_dirs("resource/qml")
    morose_auto_release()
endif(Morose_FOUND)
