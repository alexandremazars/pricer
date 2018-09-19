//
// Created by lecalvmy on 9/13/18.
//

#include "MonteCarlo.hpp"

MonteCarlo::MonteCarlo(BlackScholesModel *mod, Option *opt, PnlRng *rng, double fdStep, int nbSamples){
    mod_ = mod;
    opt_ = opt;
    rng_ = rng;
    fdStep_ = fdStep;
    nbSamples_ = nbSamples;
}
/**
    * Calcule le prix de l'option à la date 0
    *
    * @param[out] prix valeur de l'estimateur Monte Carlo
    * @param[out] ic largeur de l'intervalle de confiance
*/
void MonteCarlo::price(double &prix, double &ic){
    prix = 0;
    PnlMat *path;
    double esp_carre; //premier membre pour calculer la variance
    for (int j = 0; j < nbSamples_; ++j) {
        mod_->asset(path, opt_->T_, opt_->nbTimeSteps_, rng_);
        prix += opt_->payoff(path);
        esp_carre += pow((opt_->payoff(path)),2);
    }
    double estimateur = exp(-2*mod_->r_*opt_->T_)*((1/nbSamples_)*esp_carre-pow((1/nbSamples_)*prix,2));
    prix *= exp(-mod_->r_*opt_->T_)*(1/nbSamples_);
    ic = 2*1.96 * estimateur/sqrt(nbSamples_);
    //ic = 2 * 1.96 * sqrt(estimateur/nbSamples_);
}


void MonteCarlo::price(const PnlMat *past, double t, double &prix, double &ic){
    prix = 0;
    PnlMat *path;
    double esp_carre;
    for (int i = 0; i < nbSamples_; ++i) {
        mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
        prix += opt_->payoff(path);
        esp_carre += pow((opt_->payoff(path)),2);
    }
    double estimateur = exp(-2*mod_->r_*opt_->T_)*((1/nbSamples_)*esp_carre-pow((1/nbSamples_)*prix,2));
    prix *= exp(-mod_->r_*opt_->T_)*(1/nbSamples_);
    ic = 2*1.96 * estimateur/sqrt(nbSamples_);
}