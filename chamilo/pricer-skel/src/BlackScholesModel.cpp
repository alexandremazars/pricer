//
// Created by lecalvmy on 9/13/18.
//

#include "BlackScholesModel.hpp"

/**
     * Génère une trajectoire du modèle et la stocke dans path
     *
     * @param[out] path contient une trajectoire du modèle.
     * C'est une matrice de taille (nbTimeSteps+1) x d
     * @param[in] T  maturité
     * @param[in] nbTimeSteps nombre de dates de constatation
     */
void BlackScholesModel::asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng)
{
    pnl_mat_resize(path, nbTimeSteps + 1, size_);
    PnlVect *G = pnl_vect_new();
    pnl_rng_init(rng, PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    pnl_vect_rng_normal(G, nbTimeSteps + 1, rng);

    pnl_vect_free(&G);
    pnl_rng_free(&rng);
}