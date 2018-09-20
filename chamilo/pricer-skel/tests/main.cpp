//
// Created by teukengd on 9/19/18.
//
#include <gtest/gtest.h>
#include "test_payoff.hpp"
#include "test_exemple.hpp"
#include "asian_test_price0.hpp"
#include "basket_test_price0.hpp"
#include "basket1_test_price0.hpp"
#include "basket2_test_price0.hpp"
#include "perf_test_price0.hpp"
#include "call_test_price_t.hpp"
#include "asian_test_delta0.hpp"
#include "test_price_t.hpp"

int main(int argc,char *argv[]) {
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
