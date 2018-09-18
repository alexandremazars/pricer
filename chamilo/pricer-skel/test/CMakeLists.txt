add_executable(MonteCarlo test_mc_price.cpp)
target_link_libraries(MonteCarlo PUBLIC libsrc PUBLIC gtest)