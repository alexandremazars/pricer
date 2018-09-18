#add_executable(MonteCarlo test_mc_price.cpp)
#target_link_libraries(MonteCarlo PUBLIC libsrc PUBLIC gtest)


include_directories("${PROJECT_SOURCE_DIR}")
# First Simple Google Test
add_executable(test_price test_price.cpp)
target_link_libraries(test_price gtest gmock_main)
add_test(NAME test_price COMMAND simple)