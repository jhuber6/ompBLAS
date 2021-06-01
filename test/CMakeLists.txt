find_package(BLAS REQUIRED)

macro(add_test_file TESTNAME)
  add_executable(${TESTNAME})
  target_sources(${TESTNAME} PRIVATE ${ARGN})
  target_link_libraries(${TESTNAME} ${BLAS_LIBRARIES} ompBLAS)
  add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
endmacro()

add_test_file(dgemm dgemm.cpp)
