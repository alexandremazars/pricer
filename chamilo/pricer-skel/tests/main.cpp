//
// Created by teukengd on 9/19/18.
//
#include <gtest/gtest.h>
#include "test_payoff.hpp"
#include "test_exemple.hpp"
#include "asian_test_price0.hpp"

int main(int argc,char *argv[]) {
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
