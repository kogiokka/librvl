include(FetchContent)

get_filename_component(fetchedContentDir "../fetched-content"
    REALPATH BASE_DIR "${CMAKE_BINARY_DIR}"
)

set(FETCHCONTENT_BASE_DIR "${fetchedContentDir}")

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/fetch/FetchLibLZMA.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/fetch/FetchLZ4.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/fetch/FetchLogC.cmake")
