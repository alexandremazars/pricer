//
// Created by lecalvmy on 9/18/18.
//

#include "Option.hpp"

class PerformanceOption : public Option {
public:

    PerformanceOption(double T, int nbTimeSteps, int size);

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