//
// Created by lecalvmy on 9/18/18.
//

#include "CallOption.hpp"

double CallOption::payoff(const PnlMat *path) {
    return fmax(pnl_mat_get(path, 0, nbTimeSteps_)-strike_, 0);
}