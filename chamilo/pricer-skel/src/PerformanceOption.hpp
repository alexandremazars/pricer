//
// Created by lecalvmy on 9/18/18.
//

#include "Option.hpp"

/// \brief Option Performance hérite de la classe abstraite option
class PerformanceOption : public Option {
public:

    PerformanceOption(double T, int nbTimeSteps, int size, PnlVect* weights);
    /**
    * Constructeur de la classe
    * @param[in] double T : maturité
    * @param[in] int nbTimeSteps : nombre de pas de temps de discrétisation
    * @param[in] int size : dimension du modèle
    * @param[in] double strike : prix d'exercice de l'option
    */

    /**
     * Calcule la valeur du payoff sur la trajectoire
     *
     * @param[in] path est une matrice de taille (N+1) x d
     * contenant une trajectoire du modèle telle que créée
     * par la fonction asset.
     * @return phi(trajectoire)
     */
    double payoff(const PnlMat *path) override;
};
