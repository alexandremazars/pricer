//
// Created by lecalvmy on 9/18/18.
//

#include "PerformanceOption.hpp"

/**
* Constructeur de la classe
* @param[in]
* double T : maturité
* int nbTimeSteps : nombre de pas de temps de discrétisation
* int size : dimension du modèle
* double strike : prix d'exercice de l'option
*/
PerformanceOption::PerformanceOption(double T, int nbTimeSteps, int size){
    T_ = T;
    nbTimeSteps_ = nbTimeSteps;
    size_ = size;
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
