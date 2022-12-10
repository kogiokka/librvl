cmake_minimum_required(VERSION 3.19)

project("Read Voxels")

add_executable(read_voxels)
target_sources(read_voxels PRIVATE "read_voxels.c")
target_link_libraries(read_voxels PRIVATE rvl-test)

add_test_cwd("Read voxels" read_voxels)

