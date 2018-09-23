//
// Created by lecalvmy on 9/18/18.
//

#include "CallOption.hpp"

/**
* Constructeur de la classe
* @param[in]
* double T : maturité
* int nbTimeSteps : nombre de pas de temps de discrétisation
* int size : dimension du modèle
* double strike : prix d'exercice de l'option
*/
CallOption::CallOption(double T, int nbTimeSteps, int size, double strike){
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
double CallOption::payoff(const PnlMat *path) {
    return fmax(pnl_mat_get(path, nbTimeSteps_, 0)-strike_, 0);
}
