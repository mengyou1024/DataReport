add_library(SingleApplication STATIC IMPORTED)
set_target_properties(SingleApplication PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/lib/libSingleApplication.a")

target_include_directories(SingleApplication INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")
add_library(SingleApplication::SingleApplication ALIAS SingleApplication)
