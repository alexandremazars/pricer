//
// Created by lecalvmy on 9/18/18.
//

#include "BasketOption.hpp"

using namespace std;

BasketOption::BasketOption(double T, int nbTimeSteps, int size, double strike){
    T_ = T;
    nbTimeSteps_ = nbTimeSteps;
    size_ = size;
    strike_ = strike;
}

double BasketOption::payoff(const PnlMat *path) {
    double sum = 0;
    for (int d = 0; d < size_; ++d) {
        sum += pnl_mat_get(path, nbTimeSteps_, d);
    }
    sum = sum / size_;
    //cout << weight << "weights" <<endl;
    return fmax(sum-strike_, 0);
}