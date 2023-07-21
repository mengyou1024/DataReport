set(MOROSE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/Mutilple.ico" CACHE STRING "Morose executable icon")
set(MOROSE_OUT_DIR "${CMAKE_SOURCE_DIR}/output" CACHE STRING "Morose output directory")

set(MOROSE_DIST_DIR "${MOROSE_OUT_DIR}/dist" CACHE INTERNAL "Morose dist directory")
set(MOROSE_PLUGINS_DIR "${MOROSE_DIST_DIR}/plugins" CACHE INTERNAL "Morose plugins directory")
set(MOROSE_INSTALL_DIR "${MOROSE_OUT_DIR}/install" CACHE INTERNAL "Morose install output directory")
set(MOROSE_RUNTIME_PLUGINS_DIR "${CMAKE_CURRENT_BINARY_DIR}/plugins" CACHE INTERNAL "runtime plugins directory")
set(MOROSE_PLUGINS_TYPE "GENERIC" "VIEW" CACHE INTERNAL "Morose plugins type")
set(MOROSE_MAIN ${PROJECT_NAME} CACHE INTERNAL "morose excutable file name")
set(MOROSE_PLUGIN_QML_DIRS  CACHE INTERNAL "Morose plugin qml directories")

#[[
    `generate_exe_installer()`
    获取有关git的相关信息
    软件版本 `APP_VERSION`
    Git仓库地址 ``
    Git用户名 ``
    Git邮箱 ``
    相关变量会保存到 `morose_config.h` 文件中 
]]
macro(morose_main_setup)
    find_package(Git QUIET)
    if (GIT_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --tags
            OUTPUT_VARIABLE APP_VERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
        if (NOT APP_VERSION)
            message(FATAL_ERROR "Git repository must have a tag , use `git tag <tag_name> -m <tag_message>` to create a tag.\n"
                "\te.g.: `git tag v0.0.1 -m \"init\"`\n"
                "the git describe is use for varible `APP_VERSION`"
            )
        else()
            message(STATUS "APP VERSION :" ${APP_VERSION})
        endif()
        execute_process(
            COMMAND ${GIT_EXECUTABLE} remote
            OUTPUT_VARIABLE GIT_REMOTE
            OUTPUT_STRIP_TRAILING_WHITESPACE
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
        execute_process(
            COMMAND ${GIT_EXECUTABLE} remote get-url ${GIT_REMOTE}
            OUTPUT_VARIABLE GIT_REPOSITORY_URL
            OUTPUT_STRIP_TRAILING_WHITESPACE
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
        unset(GIT_REMOTE)
        message(STATUS "GIT_REPOSITORY_URL:${GIT_REPOSITORY_URL}")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} config user.name
            OUTPUT_VARIABLE GIT_USER_NAME
            OUTPUT_STRIP_TRAILING_WHITESPACE
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
        message(STATUS "GIT_USER_NAME:${GIT_USER_NAME}")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} config user.email
            OUTPUT_VARIABLE GIT_USER_EMAIL
            OUTPUT_STRIP_TRAILING_WHITESPACE
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
        message(STATUS "GIT_USER_EMAIL:${GIT_USER_EMAIL}")
    else()
        message(WARNING "no git found, please install git: https://git-scm.com/")
    endif()
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/morose_config.h.in
        ${CMAKE_CURRENT_SOURCE_DIR}/morose_config.h
        @ONLY
    )
endmacro(morose_main_setup)

#[[
    `morose_auto_release()`
    自动打包和生成发布程序
    需要生成 `generate_exe_installer` 目标
]]
function(morose_auto_release)
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        # 生成inno setup 编译脚本
        configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/script/pack-installer.iss.in
            ${MOROSE_OUT_DIR}/pack-installer.iss
            @ONLY
        )

        foreach(ITEM ${MOROSE_PLUGINS_TYPE})
            string(TOLOWER ${ITEM} DIR_NAME)
            set(CREATE_DIRS ${CREATE_DIRS} ${MOROSE_PLUGINS_DIR}/${DIR_NAME})
        endforeach(ITEM ${MOROSE_PLUGINS_TYPE})

        # 清除输出
        add_custom_target(
            bundle_clean
            COMMAND ${CMAKE_COMMAND} -E rm -rf "${MOROSE_DIST_DIR}"
            COMMAND ${CMAKE_COMMAND} -E rm -rf "${MOROSE_INSTALL_DIR}"
            COMMENT "clear output directory"
        )

        foreach(ITEM ${MOROSE_PLUGINS_TYPE})
            string(TOLOWER ${ITEM} DIR_NAME)
            set(PLUGIN_DIRS ${PLUGIN_DIRS} ${MOROSE_PLUGINS_DIR}/${DIR_NAME})
        endforeach(ITEM ${MOROSE_PLUGINS_TYPE})

        foreach(ITEM ${MOROSE_PLUGIN_QML_DIRS})
            set(QML_DIRS ${QML_DIRS} "-qmldir=${ITEM}")
        endforeach(ITEM ${MOROSE_PLUGIN_QML_DIRS})

        # 搜索inno setup工具
        find_program(ISCC_PATH ISCC)
        if (ISCC_PATH)
            message(STATUS "Detected ISCC_PATH: ${ISCC_PATH}")

            add_custom_target(
                generate_exe_installer
                # 创建目录 防止某一类插件未使用
                COMMAND ${CMAKE_COMMAND} -E make_directory ${PLUGIN_DIRS}
                COMMAND ${CMAKE_COMMAND} -E make_directory "${MOROSE_DIST_DIR}/FluentUI"
                # 拷贝生成的EXE
                COMMAND ${CMAKE_COMMAND} -E copy
                "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}"
                "${MOROSE_DIST_DIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}"
                COMMAND echo "copy output file:${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}"
                # 执行windeployqt进行打包
                COMMAND echo "windeployqt ..."
                COMMAND ${WINDEPLOYQT_EXECUTABLE}
                --verbose 0
                ${MOROSE_DIST_DIR}
                ${PLUGIN_DIRS}
                ${QML_DIRS}
                # 执行ISCC进行打包
                COMMAND echo "inno setup generate executable installer ..."
                COMMAND ${ISCC_PATH} "${MOROSE_OUT_DIR}/pack-installer.iss" /Qp
                DEPENDS ${MOROSE_OUT_DIR}/pack-installer.iss ${PROJECT_NAME}
                COMMENT "generated executable installer: ${MOROSE_INSTALL_DIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}"
            )
        else(ISCC_PATH)
            message(WARNING "no ISCC path found, please install inno setup and add it to path\n see: https://jrsoftware.org/isinfo.php")
            add_custom_target(
                generate_exe_installer
                # 创建目录 防止某一类插件未使用
                COMMAND ${CMAKE_COMMAND} -E make_directory ${PLUGIN_DIRS}
                COMMAND ${CMAKE_COMMAND} -E make_directory "${MOROSE_DIST_DIR}/FluentUI"
                # 拷贝生成的EXE
                COMMAND ${CMAKE_COMMAND} -E copy
                "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}"
                "${MOROSE_DIST_DIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}"
                COMMAND echo "copy output file:${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}"
                # 执行windeployqt进行打包
                COMMAND echo "windeployqt ..."
                COMMAND ${WINDEPLOYQT_EXECUTABLE}
                --verbose 0
                ${MOROSE_DIST_DIR}
                ${PLUGIN_DIRS}
                ${QML_DIRS}
            )
        endif(ISCC_PATH)
    endif(CMAKE_BUILD_TYPE STREQUAL "Release")
endfunction(morose_auto_release)

#[[
    `morose_add_plugins(PATH <path> [TARGET] target)`
    添加插件的构建目录
    TARGET 参数为插件的构建目标
]]
function(morose_add_plugins)
    set(oneValueArgs PATH TARGET)
    cmake_parse_arguments(MOROSE_ADD_PLUGINS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    add_subdirectory(${MOROSE_ADD_PLUGINS_PATH})
    if (NOT MOROSE_ADD_PLUGINS_TARGET)
        get_filename_component(MOROSE_ADD_PLUGINS_TARGET ${PATH} NAME)
    endif(NOT MOROSE_ADD_PLUGINS_TARGET)
    message(STATUS "MOROSE_MAIN:${MOROSE_MAIN}, add plugin:[${MOROSE_ADD_PLUGINS_TARGET}]")
    add_dependencies(${MOROSE_MAIN} ${MOROSE_ADD_PLUGINS_TARGET})
endfunction(morose_add_plugins)

#[[
    `morose_add_qml_dirs(...)`
    添加qml目录，用于`windeployqt`调用
    如果参数为空则将当前cmake源文件目录添加进去
]]
function(morose_add_qml_dirs)
    if (ARGC EQUAL 0)
        set(MOROSE_PLUGIN_QML_DIRS ${MOROSE_PLUGIN_QML_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "Morose plugin qml directories")
    else(ARGC EQUAL 0)
        foreach(ITEM ${ARGV})
            set(MOROSE_PLUGIN_QML_DIRS ${MOROSE_PLUGIN_QML_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/${ITEM} CACHE INTERNAL "Morose plugin qml directories")
        endforeach(ITEM ${ARGV})
    endif()
endfunction(morose_add_qml_dirs)

#[[
    `morose_plugin_setup([TYPE] type)`
    设置当前项目为插件项目
    type 为插件类型，默认为GENERIC 类型必须是`MOROSE_PLUGINS_TYPE`中的一个
]]
macro(morose_plugin_setup)
    set(oneValueArgs "TYPE")
    cmake_parse_arguments(SETUP "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if (SETUP_TYPE)
        if (NOT $<IN_LIST:${SETUP_TYPE}:${MOROSE_PLUGINS_TYPE}>)
            message(FATAL_ERROR "type must one of {${MOROSE_PLUGINS_TYPE}}")
        endif(NOT $<IN_LIST:${SETUP_TYPE}:${MOROSE_PLUGINS_TYPE}>)
    else(SETUP_TYPE)
        set(SETUP_TYPE "GENERIC")
    endif(SETUP_TYPE)
    string(TOLOWER ${SETUP_TYPE} SETUP_TYPE)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}"
        "${MOROSE_RUNTIME_PLUGINS_DIR}/${SETUP_TYPE}/${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}"
        COMMAND echo "copy plugin [${SETUP_TYPE}/${PROJECT_NAME}] to runtime directory"
        COMMENT "copy plugin [${SETUP_TYPE}/${PROJECT_NAME}] to runtime directory"
    )
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}"
            "${MOROSE_PLUGINS_DIR}/${SETUP_TYPE}/${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}"
            COMMAND echo "copy plugin [${SETUP_TYPE}/${PROJECT_NAME}] to bundle directory"
            COMMENT "copy plugin [${SETUP_TYPE}/${PROJECT_NAME}] to bundle directory"
        )
    endif(CMAKE_BUILD_TYPE STREQUAL "Release")
endmacro(morose_plugin_setup)

set(Morose_FOUND TRUE)
