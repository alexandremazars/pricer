//
// Created by lecalvmy on 9/18/18.
//

#include "BasketOption.hpp"

double BasketOption::payoff(const PnlMat *path) {
    const double weight = 1/size_;
    double sum = 0;
    for (int d = 0; d < size_; ++d) {
        sum += weight*pnl_mat_get(path, d, nbTimeSteps_);
    }
    return fmax(sum-strike_, 0);
}