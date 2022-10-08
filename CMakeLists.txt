cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project("librvl"
    DESCRIPTION "The Regular VoLumetric format reference library"
    LANGUAGES C)

add_library(librvl SHARED)

include("cmake/FetchLZ4.cmake")

set_target_properties(librvl PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
)

target_compile_options(librvl PRIVATE
    $<$<C_COMPILER_ID:GNU,CLANG>:-Wall -Wextra -pedantic-errors>
)

target_sources(librvl PRIVATE
    "src/rvl_write.c"
    "src/rvl_read.c"
    "src/rvl_common.c"
    "src/rvl_info.c"
    "src/rvl_text.c"
    "src/rvl_data.c"

    "src/detail/rvl_write_p.c"
    "src/detail/rvl_read_p.c"
)

target_include_directories(librvl PRIVATE "include/")

target_link_libraries(librvl
PRIVATE
    lz4_static
)

# CTest
include(CTest)
enable_testing()
add_subdirectory(test)
