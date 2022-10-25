add_executable(example)

target_sources(example PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/examples/example.c")
target_include_directories(example PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(example PRIVATE rvl::rvl)
