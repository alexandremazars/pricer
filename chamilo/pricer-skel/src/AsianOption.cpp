//
// Created by lecalvmy on 9/18/18.
//

#include "AsianOption.hpp"

/**
* Constructeur de la classe
* @param[in]
* double T : maturité
* int nbTimeSteps : nombre de pas de temps de discrétisation
* int size : dimension du modèle
* double strike : prix d'exercice de l'option
*/
AsianOption::AsianOption(double T, int nbTimeSteps, int size, double strike){
    T_ = T;
    nbTimeSteps_ = nbTimeSteps;
    size_ = size;
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
