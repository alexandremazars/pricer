//
// Created by teukengd on 9/19/18.
//
#pragma once


#ifndef CHAMILO_TEST_ASSET_H
#define CHAMILO_TEST_ASSET_H
#include <gtest/gtest.h>
#include "pnl/pnl_matrix.h"
#include "../src/CallOption.cpp"
#include "../src/BasketOption.hpp"
#include <fstream>
#include <iostream>


using namespace std;

TEST(CallOption, callOptionPos){
    //PnlMat *callPath = pnl_mat_create_from_file("../../market-data/simul_call.dat");
    const char *file = "../../market-data/simul_call.dat";
    PnlMat *callPath = pnl_mat_create_from_file(file);
    CallOption *call  = new CallOption(1, 365, 1,98);
    double result = call->payoff(callPath);
    ASSERT_TRUE(result >= 16.088379 - 0.0001 && result <= 16.088379 + 0.0001);
}

TEST(CallOption, callOptionNeg){
    //PnlMat *callPath = pnl_mat_create_from_file("../../market-data/simul_call.dat");
    const char *file = "../../market-data/simul_call.dat";
    PnlMat *callPath = pnl_mat_create_from_file(file);
    CallOption *call  = new CallOption(1, 365, 1,118);
    double result = call->payoff(callPath);
    ASSERT_EQ(result, 0);
}

TEST(BasketOption, BasketOption2dPos){
    //PnlMat *callPath = pnl_mat_create_from_file("../../market-data/simul_call.dat");
    const char *file = "../../market-data/simul_basket_2d.dat";
    PnlMat *callPath = pnl_mat_create_from_file(file);
    CallOption *call  = new CallOption(1, 365, 1,100);
    double result = call->payoff(callPath);
    ASSERT_TRUE(result >= 32.169739 - 0.0001 && result <= 32.169739 + 0.0001);
}

TEST(BasketOption, BasketOption2dNeg){
    //PnlMat *callPath = pnl_mat_create_from_file("../../market-data/simul_call.dat");
    const char *file = "../../market-data/simul_basket_2d.dat";
    PnlMat *callPath = pnl_mat_create_from_file(file);
    CallOption *call  = new CallOption(1, 365, 1,134);
    double result = call->payoff(callPath);
    ASSERT_EQ(result, 0);
}


#endif //CHAMILO_TEST_ASSET_H
