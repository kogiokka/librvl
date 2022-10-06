add_executable(unit_tests)

target_sources(unit_tests PRIVATE
  unit_tests.c
  rvl_tests.c
)

target_include_directories(unit_tests PRIVATE ../src)
target_link_libraries(unit_tests PRIVATE librvl)
add_test(NAME "Write INFO" COMMAND unit_tests 1)
add_test(NAME "Write DATA" COMMAND unit_tests 2)
add_test(NAME "Write TEXT" COMMAND unit_tests 3)
