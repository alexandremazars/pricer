//
// Created by lecalvmy on 9/13/18.
//

#include "BlackScholesModel.hpp"

BlackScholesModel::BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot){
    size_ = size;
    r_ = r;
    rho_ = rho;
    sigma_ = sigma;
    spot_ = spot;
}

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
    double delta_t = (nbTimeSteps + 1)/T;

    //Remplir la mtrice de correlation
    PnlMat *mat_Cor = pnl_mat_create_from_scalar(size_, size_, rho_ );
    for (int i = 0; i < size_; ++i) {
        pnl_mat_set(mat_Cor, i, i, 1);
    }

    //Matrice de Cholesky
    PnlMat *mat_Chol = pnl_mat_copy(mat_Cor);
    pnl_mat_chol(mat_Chol);

    pnl_mat_resize(path, nbTimeSteps + 1, size_);

    //Simuler vecteurs gaussiens
    PnlMat *suite_Gauss = pnl_mat_create(size_, nbTimeSteps + 1);
    PnlVect *G = pnl_vect_new();
    for (int d = 0; d < size_; ++d) {
        G = pnl_vect_new();
        pnl_rng_init(rng, PNL_RNG_MERSENNE);
        pnl_rng_sseed(rng, time(NULL));
        pnl_vect_rng_normal(G, nbTimeSteps + 1, rng);
        pnl_mat_set_row(suite_Gauss, G, d);
    }


    //Calcul des prix
    PnlVect *row_Chol;
    PnlVect *col_Gauss;
    for (int d = 0; d < size_ ; ++d) {
        double prix_Prec = pnl_vect_get(spot_, d);
        double prix = prix_Prec;
        pnl_mat_set(path, 0, d, prix_Prec);
        double sigma = pnl_vect_get(sigma_, d);
        pnl_mat_get_row(row_Chol, mat_Chol, d);
        for (int i = 1; i < nbTimeSteps+1; ++i) {
            pnl_mat_get_col(col_Gauss, suite_Gauss, i);
            pnl_vect_mult_vect_term(row_Chol, col_Gauss);
            prix = prix_Prec * exp((r_-(pow(sigma,2))/2)/delta_t + pnl_vect_get(row_Chol, 0) * sigma * sqrt(delta_t));
            pnl_mat_set(path, i, d, prix);
            prix_Prec = prix;
        }
    }

    // Free
    pnl_vect_free(&row_Chol);
    pnl_vect_free(&col_Gauss);
    pnl_mat_free(&suite_Gauss);
    pnl_mat_free(&mat_Chol);
    pnl_mat_free(&mat_Cor);
    pnl_vect_free(&G);
    pnl_rng_free(&rng);
}

void BlackScholesModel::asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past){
    double delta_t = (nbTimeSteps + 1)/T;
    //Remplir la mtrice de correlation
    PnlMat *mat_Cor = pnl_mat_create_from_scalar(size_, size_, rho_ );
    for (int i = 0; i < size_; ++i) {
        pnl_mat_set(mat_Cor, i, i, 1);
    }

    //Matrice de Cholesky
    PnlMat *mat_Chol = pnl_mat_copy(mat_Cor);
    pnl_mat_chol(mat_Chol);

    pnl_mat_resize(path, nbTimeSteps + 1, size_);

    //simuler les vecteurs gaussien
    PnlMat *suite_Gauss = pnl_mat_create(size_, nbTimeSteps + 1);
    PnlVect *G = pnl_vect_new();
    for (int d = 0; d < size_; ++d) {
        G = pnl_vect_new();
        pnl_rng_init(rng, PNL_RNG_MERSENNE);
        pnl_rng_sseed(rng, time(NULL));
        pnl_vect_rng_normal(G, nbTimeSteps - t, rng);
        pnl_mat_set_row(suite_Gauss, G, d);
    }
    //Calcul des prix
    PnlVect *row_Chol;
    PnlVect *col_Gauss;

    //mettre dans path les valeurs de past
    for (int j = 0; j <= t; ++j) {
        PnlVect *price_i;
        pnl_mat_get_row(price_i, past, j);
        pnl_mat_set_row(path, price_i, j);
    }

    //Simulées les valeurs suivantes

    for (int d = 0; d < size_ ; ++d) {
        double prix_Prec = pnl_mat_get(past,t,d);
        double prix = prix_Prec;
        double sigma = pnl_vect_get(sigma_, d);
        pnl_mat_get_row(row_Chol, mat_Chol, d);

        for (int i = 0; i < nbTimeSteps - t ; ++i) {
            pnl_mat_get_col(col_Gauss, suite_Gauss, i);
            pnl_vect_mult_vect_term(row_Chol, col_Gauss);
            prix = exp((r_-(pow(sigma,2))/2)/delta_t + pnl_vect_get(row_Chol, 0) * sigma * sqrt(delta_t));
            prix = prix * prix_Prec;
            pnl_mat_set(path, (i+t+1), d, prix);
        }
    }


}
