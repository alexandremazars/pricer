//
// Created by teukengd on 9/19/18.
//
#pragma once


#ifndef CHAMILO_TEST_ASSET_H
#define CHAMILO_TEST_ASSET_H
#include <gtest/gtest.h>
#include "pnl/pnl_matrix.h"
#include "../src/CallOption.cpp"
#include "../src/BasketOption.cpp"
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

TEST(CallOption, callOptionNul){
    const char *file = "../../market-data/simul_call.dat";
    PnlMat *callPath = pnl_mat_create_from_file(file);
    CallOption *call  = new CallOption(1, 365, 1,118);
    double result = call->payoff(callPath);
    ASSERT_EQ(result, 0);
}

TEST(BasketOption, BasketOption2dPos){
    const char *file = "../../market-data/simul_basket_2d.dat";
    PnlMat *basketPath = pnl_mat_create_from_file(file);
    BasketOption *basket  = new BasketOption(1, 365, 2, 100);
    double result = basket->payoff(basketPath);
    cout << result << endl;
    ASSERT_TRUE(result >= 15.614674 - 0.0001 && result <= 15.614674 + 0.0001);
}

TEST(BasketOption, BasketOption2dNul){
    const char *file = "../../market-data/simul_basket_2d.dat";
    PnlMat *basketPath = pnl_mat_create_from_file(file);
    BasketOption *basket  = new BasketOption(1, 365, 2, 120);
    double result = basket->payoff(basketPath);
    ASSERT_EQ(result,0);
}




#endif //CHAMILO_TEST_ASSET_H
