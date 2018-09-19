//
// Created by lecalvmy on 9/18/18.
//

#include "PerformanceOption.hpp"


PerformanceOption::PerformanceOption(double T, int nbTimeSteps, int size){
    T_ = T;
    nbTimeSteps_ = nbTimeSteps;
    size_ = size;
}

double PerformanceOption::payoff(const PnlMat *path) {
    double sum_N = 0;
    for (int i = 1; i < nbTimeSteps_ + 1 ; ++i) {
        double sum_num = 0;
        double sum_denom = 0;
        for (int d = 0; d < size_; ++d) {
            sum_num += pnl_mat_get(path, i, d);
            sum_denom += pnl_mat_get(path, i-1, d);
        }
        sum_N += fmax(sum_num/sum_denom - 1, 0);
    }
    return 1 + sum_N;
}