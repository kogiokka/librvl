if(NOT BUILD_TESTING)
  return()
endif()

add_library(rvl-test INTERFACE)
target_link_libraries(rvl-test INTERFACE rvl::rvl)

if ((CMAKE_C_COMPILER_ID STREQUAL "GNU" AND NOT MINGW) OR
    CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_compile_options(rvl-test INTERFACE -fsanitize=address)
    target_link_options(rvl-test INTERFACE -fsanitize=address)
endif()

add_subdirectory("create_destroy")
add_subdirectory("input_output")
add_subdirectory("lzma2")
add_subdirectory("lz4")
add_subdirectory("tiny_data")
add_subdirectory("read_info")
