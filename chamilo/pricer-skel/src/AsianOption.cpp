//
// Created by lecalvmy on 9/18/18.
//

#include "AsianOption.hpp"

AsianOption::AsianOption(double T, int nbTimeSteps, int size, double strike){
    T_ = T;
    nbTimeSteps_ = nbTimeSteps;
    size_ = size;
    strike_ = strike;
}

double AsianOption::payoff(const PnlMat *path) {
    const double weight = 1/size_;
    double sum = 0;
    for (int d = 0; d < size_; ++d) {
        double sum_N = 0;
        for (int i = 0; i < nbTimeSteps_ + 1; ++i) {
            sum_N += pnl_mat_get(path, d, i);
        }
        sum += 1/(nbTimeSteps_+1) * sum_N * weight;
    }
    return fmax(sum-strike_, 0);
}