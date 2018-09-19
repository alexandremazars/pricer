#add_executable(MonteCarlo test_mc_price.cpp)
#target_link_libraries(MonteCarlo PUBLIC libsrc PUBLIC gtest)


#include_directories("${PROJECT_SOURCE_DIR}")
# First Simple Google Test
add_executable(test main.cpp ${SOURCESTEST} ${HEADERSTEST})
target_link_libraries(test gtest gtest_main ${LIBS})
#add_test(NAME test COMMAND simple)