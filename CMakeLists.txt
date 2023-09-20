cmake_minimum_required(VERSION 3.12)

project(DataReport VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

function(auto_add_subdirecory directory)
    file(GLOB THIRDS_PATH "${directory}/*")
    foreach(THIRD ${THIRDS_PATH})
        if (IS_DIRECTORY "${THIRD}" AND EXISTS "${THIRD}/CMakeLists.txt")
            file(RELATIVE_PATH THIRD_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${THIRD})
            add_subdirectory(${THIRD_PATH})
        endif(IS_DIRECTORY "${THIRD}" AND EXISTS "${THIRD}/CMakeLists.txt")
    endforeach(THIRD ${THIRD_PATH})
endfunction(auto_add_subdirecory directory)

auto_add_subdirecory("3rdparty")
add_subdirectory("3rdparty/QXlsx/QXlsx")

option(MOROSE_TEST "run test on main" OFF)

if (MOROSE_TEST)
    add_compile_definitions("MOROSE_TEST")
endif(MOROSE_TEST)

add_compile_definitions("QAPPLICATION_CLASS=QGuiApplication")

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
    common/qframelesswindow.h
    common/qframelesswindow.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Quick QXlsx::QXlsx SingleApplication::SingleApplication dwmapi user32)

set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Morose)

if (Morose_FOUND)
    morose_add_qml_dirs("resource/qml")
    morose_copy(
        DIRS "templates/log"
    )
    if((NOT EXISTS "${CMAKE_SOURCE_DIR}/config/ProductConfig.json") AND (NOT CMAKE_BUILD_TYPE STREQUAL "Release"))
        message(FATAL_ERROR "no ProductConfig.json file please see ${CMAKE_SOURCE_DIR}/config/ProductConfigTemplate.json")
    endif((NOT EXISTS "${CMAKE_SOURCE_DIR}/config/ProductConfig.json") AND (NOT CMAKE_BUILD_TYPE STREQUAL "Release"))

    morose_add_environment_config_file(
        TARGET ${PROJECT_NAME}
        DEPLOY "config/DeployConfig.json"
        PRODUCT "config/ProductConfig.json"
        DIST "Config.json"
    )
    morose_auto_release()
endif(Morose_FOUND)
