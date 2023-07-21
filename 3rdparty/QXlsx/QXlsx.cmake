add_library(QXlsx STATIC IMPORTED)
set_target_properties(QXlsx PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/lib/libQXlsxQt6.a")
target_include_directories(QXlsx INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")
add_library(QXlsx::QXlsx ALIAS QXlsx)
