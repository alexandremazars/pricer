//
// Created by lecalvmy on 9/18/18.
//

#include "CallOption.hpp"

CallOption::CallOption(double T, int nbTimeSteps, int size, PnlVect* weights, double strike){
    T_ = T;
    nbTimeSteps_ = nbTimeSteps;
    size_ = size;
    weights_ = weights;
    strike_ = strike;
}

double CallOption::payoff(const PnlMat *path) {
    return fmax(pnl_mat_get(path, path->m - 1, 0)-strike_, 0);
}
