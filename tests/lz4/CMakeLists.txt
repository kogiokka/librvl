cmake_minimum_required(VERSION 3.19)

project("LZ4 Compression")

add_executable(test-lz4)
target_sources(test-lz4 PRIVATE "lz4.c")
target_link_libraries(test-lz4 PRIVATE rvl-test)

add_test(NAME "LZ4 compression method" COMMAND test-lz4)
