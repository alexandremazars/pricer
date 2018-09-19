//
// Created by lecalvmy on 9/18/18.
//

#include "CallOption.hpp"

CallOption::CallOption(double T, int nbTimeSteps, int size, double strike){
    T_ = T;
    nbTimeSteps_ = nbTimeSteps;
    size_ = size;
    strike_ = strike;
}

double CallOption::payoff(const PnlMat *path) {
    return fmax(pnl_mat_get(path, nbTimeSteps_, 0)-strike_, 0);
}