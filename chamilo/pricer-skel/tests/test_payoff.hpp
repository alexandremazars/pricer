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
#include "../src/AsianOption.cpp"
#include "../src/PerformanceOption.cpp"
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

TEST(BasketOption, BasketOption1Pos){
    const char *file = "../../market-data/simul_basket_1.dat";
    PnlMat *basketPath = pnl_mat_create_from_file(file);
    BasketOption *basket  = new BasketOption(3, 1000, 40, 100);
    double result = basket->payoff(basketPath);
    ASSERT_TRUE(result >= 3.79563912499998 - 0.0001 && result <= 3.79563912499998 + 0.0001);
}

TEST(BasketOption, BasketOption1Nul){
    const char *file = "../../market-data/simul_basket_1.dat";
    PnlMat *basketPath = pnl_mat_create_from_file(file);
    BasketOption *basket  = new BasketOption(3, 1000, 40, 110);
    double result = basket->payoff(basketPath);
    ASSERT_EQ(result,0);
}

TEST(BasketOption, BasketOption2Pos){
    const char *file = "../../market-data/simul_basket_2.dat";
    PnlMat *basketPath = pnl_mat_create_from_file(file);
    BasketOption *basket  = new BasketOption(1, 365, 40, 100);
    double result = basket->payoff(basketPath);
    ASSERT_TRUE(result >= 35.6169229500001 - 0.0001 && result <= 35.6169229500001 + 0.0001);
}

TEST(BasketOption, BasketOption2Nul){
    const char *file = "../../market-data/simul_basket_2.dat";
    PnlMat *basketPath = pnl_mat_create_from_file(file);
    BasketOption *basket  = new BasketOption(1, 365, 40, 140);
    double result = basket->payoff(basketPath);
    ASSERT_EQ(result,0);
}

TEST(AsianOption, AsianOptionPos){
    const char *file = "../../market-data/simul_asian.dat";
    PnlMat *asianPath = pnl_mat_create_from_file(file);
    AsianOption *asian  = new AsianOption(1.5, 450, 2, 90);
    double result = asian->payoff(asianPath);
    ASSERT_TRUE(result >= 2.1552890886918 - 0.0001 && result <= 2.1552890886918 + 0.0001);
}

TEST(AsianOption, AsianOptionNul){
    const char *file = "../../market-data/simul_asian.dat";
    PnlMat *asianPath = pnl_mat_create_from_file(file);
    AsianOption *asian  = new AsianOption(1.5, 450, 2, 95);
    double result = asian->payoff(asianPath);
    ASSERT_EQ(result,0);
}

TEST(PerformanceOption, PerformanceOptionPos){
    const char *file = "../../market-data/simul_perf.dat";
    PnlMat *perfPath = pnl_mat_create_from_file(file);
    PerformanceOption *perf  = new PerformanceOption(2, 96, 5);
    double result = perf->payoff(perfPath);
    ASSERT_TRUE(result >= 1.88337442199339 - 0.0001 && result <= 1.88337442199339 + 0.0001);
}



#endif //CHAMILO_TEST_ASSET_H
