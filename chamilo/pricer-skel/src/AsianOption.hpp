//
// Created by lecalvmy on 9/18/18.
//

#include "Option.hpp"

class AsianOption : public Option
{
public:

    double strike_;

    AsianOption(double T, int nbTimeSteps, int size, PnlVect* weights, double strike);

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
