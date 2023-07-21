if (CMAKE_PREFIX_PATH)
    if (EXISTS "${CMAKE_PREFIX_PATH}/qml/FluentUI")
        message(STATUS "${CMAKE_PREFIX_PATH}/qml/FluentUI is exists!")
    else(EXISTS "${CMAKE_PREFIX_PATH}/qml/FluentUI")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_LIST_DIR}/FluentUI"
            "${CMAKE_PREFIX_PATH}/qml/FluentUI"
            COMMAND echo "copy FluentUI plugin to qml directory"
        )
    endif(EXISTS "${CMAKE_PREFIX_PATH}/qml/FluentUI")
else(CMAKE_PREFIX_PATH)
    message(FATAL_ERROR "CMAKE_PREFIX_PATH is None")
endif(CMAKE_PREFIX_PATH)

