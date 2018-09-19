//
// Created by lecalvmy on 9/18/18.
//

#include "AsianOption.hpp"

using namespace std;

AsianOption::AsianOption(double T, int nbTimeSteps, int size, double strike){
    T_ = T;
    nbTimeSteps_ = nbTimeSteps;
    size_ = size;
    strike_ = strike;
}

double AsianOption::payoff(const PnlMat *path) {
    double sum = 0;
    double sum_N;
    for (int d = 0; d < size_; ++d) {
        sum_N = 0;
        for (int i = 0; i < nbTimeSteps_ + 1; ++i) {
            sum_N += pnl_mat_get(path, i, d);
        }
        sum += sum_N / (size_ * (nbTimeSteps_+1) );
    }
    return fmax(sum-strike_, 0);
}