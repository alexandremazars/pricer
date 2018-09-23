//
// Created by lecalvmy on 9/18/18.
//

#include "BasketOption.hpp"

/**
* Constructeur de la classe
* @param[in]  T : maturité
* @param[in]  nbTimeSteps : nombre de pas de temps de discrétisation
* @param[in]  size : dimension du modèle
* @param[in] weights : poids des actifs
* @param[in]  strike : prix d'exercice de l'option
*/
BasketOption::BasketOption(double T, int nbTimeSteps, int size, PnlVect* weights, double strike){
    T_ = T;
    nbTimeSteps_ = nbTimeSteps;
    size_ = size;
    weights_ = weights;
    strike_ = strike;
}

/**
 * Calcule la valeur du payoff sur la trajectoire
 *
 * @param[in] path est une matrice de taille (N+1) x d
 * contenant une trajectoire du modèle telle que créée
 * par la fonction asset.
 * @return phi(trajectoire)
 */
double BasketOption::payoff(const PnlMat *path) {
    double sum = 0;
    for (int d = 0; d < size_; ++d) {
        sum += pnl_mat_get(path, path->m - 1, d);
    }
    sum = sum / size_;
    return fmax(sum-strike_, 0);
}
