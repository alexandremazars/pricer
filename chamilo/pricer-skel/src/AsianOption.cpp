//
// Created by lecalvmy on 9/18/18.
//

#include "AsianOption.hpp"

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