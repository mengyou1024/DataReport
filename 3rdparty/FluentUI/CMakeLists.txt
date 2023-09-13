cmake_minimum_required(VERSION 3.15)
project(FluentUI)

if (CMAKE_PREFIX_PATH)
    if (EXISTS "${CMAKE_PREFIX_PATH}/qml/FluentUI")
        message(STATUS "${CMAKE_PREFIX_PATH}/qml/FluentUI is exists!")
    else(EXISTS "${CMAKE_PREFIX_PATH}/qml/FluentUI")
        message(STATUS "${CMAKE_PREFIX_PATH}/qml/FluentUI not exists!")
        message(STATUS "will copy FluentUi to ${CMAKE_PREFIX_PATH}/qml/FluentUI")
        message(STATUS "copy ${CMAKE_CURRENT_LIST_DIR}/FluentUI to ${CMAKE_PREFIX_PATH}/qml/FluentUI")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_LIST_DIR}/FluentUI"
            "${CMAKE_PREFIX_PATH}/qml/FluentUI"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif(EXISTS "${CMAKE_PREFIX_PATH}/qml/FluentUI")
else(CMAKE_PREFIX_PATH)
    message(FATAL_ERROR "CMAKE_PREFIX_PATH is None")
endif(CMAKE_PREFIX_PATH)

