#############################################################################
# liblzma
#############################################################################

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build liblzma as a shared library instead of static")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/xz-5.2.7")
set_target_properties(liblzma PROPERTIES POSITION_INDEPENDENT_CODE ON)


#############################################################################
# lz4
#############################################################################

add_library(lz4 STATIC)

set(LZ4_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/lz4-1.9.4")

set_target_properties(lz4 PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(lz4 PUBLIC "${LZ4_SOURCE_DIR}")
target_sources(lz4
PRIVATE
   "${LZ4_SOURCE_DIR}/lz4.c"
   "${LZ4_SOURCE_DIR}/lz4file.c"
   "${LZ4_SOURCE_DIR}/lz4frame.c"
   "${LZ4_SOURCE_DIR}/lz4hc.c"
   "${LZ4_SOURCE_DIR}/xxhash.c"
)

#############################################################################
# log.c
#############################################################################

add_library(logc STATIC)

set(LOGC_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/log.c")

target_sources(logc PRIVATE "${LOGC_SOURCE_DIR}/log.c")
target_include_directories(logc PUBLIC "${LOGC_SOURCE_DIR}")
target_compile_definitions(logc PRIVATE LOG_USE_COLOR)
set_target_properties(logc PROPERTIES POSITION_INDEPENDENT_CODE ON)
