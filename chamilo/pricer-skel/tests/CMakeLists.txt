#add_executable(MonteCarlo test_mc_price.cpp)
#target_link_libraries(MonteCarlo PUBLIC libsrc PUBLIC gtest)


#include_directories("${PROJECT_SOURCE_DIR}")
# First Simple Google Test
add_executable(test1 main.cpp ${SOURCESTEST} ${HEADERSTEST})
target_link_libraries(test1 gtest gtest_main ${LIBS})
target_link_libraries(test1 jl_parser ${LIBS})

#add_test(NAME test COMMAND simple)