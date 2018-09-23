//
// Created by lecalvmy on 9/18/18.
//

#include "PerformanceOption.hpp"

/**
* Constructeur de la classe
* @param[in] T : maturité
* @param[in] nbTimeSteps : nombre de pas de temps de discrétisation
* @param[in] size : dimension du modèle
* @param[in] weights : poids des actifs
*/
PerformanceOption::PerformanceOption(double T, int nbTimeSteps, int size, PnlVect* weights){
    T_ = T;
    nbTimeSteps_ = nbTimeSteps;
    size_ = size;
    weights_ = weights;
}

/**
 * Calcule la valeur du payoff sur la trajectoire
 *
 * @param[in] path est une matrice de taille (N+1) x d
 * contenant une trajectoire du modèle telle que créée
 * par la fonction asset.
 * @return phi(trajectoire)
 */
double PerformanceOption::payoff(const PnlMat *path) {
    double sum_N = 0;
    for (int i = 1; i < path->m ; ++i) {
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
