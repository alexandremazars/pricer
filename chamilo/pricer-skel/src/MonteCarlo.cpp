//
// Created by lecalvmy on 9/13/18.
//

#include "MonteCarlo.hpp"


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
    ic = 1.96 * estimateur/sqrt(nbSamples_);
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
    ic = 1.96 * estimateur/sqrt(nbSamples_);
}

void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta, const PnlVect *conf_delta){
    pnl_vect_resize(delta, mod_->size_);

    int timestep = (opt_->nbTimeSteps_+1)/opt_->T_;
    int sum = 0;
    double prix;
    double ic;
    price(past, t, prix, ic);
    double coefficient = exp(-mod_->r_ * (opt_->T_ - t))/(2 * nbSamples_ * fdStep_ * prix);

    PnlMat *path;
    PnlMat *increment_path;
    PnlMat *decrement_path;

    for (int d = 0; d < mod_->size_; d++) {
      for (int i = 0; i < nbSamples_; i++) {
        mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
        mod_->shiftAsset(increment_path, path, d, fdStep_, t, timestep);
        mod_->shiftAsset(decrement_path, path, d, -fdStep_, t, timestep);
        sum += opt_->payoff(increment_path) - opt_->payoff(decrement_path);
        mod_->asset(increment_path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
        mod_->asset(decrement_path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
      }
      pnl_vect_set(delta, d, coefficient * sum);
    }
}
