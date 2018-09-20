//
// Created by lecalvmy on 9/13/18.
//

#include "MonteCarlo.hpp"
using namespace std;

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
    double esp_carre = 0; //premier membre pour calculer la variance
    path = pnl_mat_create(opt_->nbTimeSteps_ + 1, mod_->size_);
    for (size_t j = 0; j < nbSamples_; ++j) {
        mod_->asset(path, opt_->T_, opt_->nbTimeSteps_, rng_);
        prix += opt_->payoff(path);
        esp_carre += pow((opt_->payoff(path)),2);
    }
    double estimateur_carre = exp(-2*mod_->r_*opt_->T_)*(esp_carre/nbSamples_-pow(prix/nbSamples_,2));
    prix *= exp(-mod_->r_*opt_->T_)/nbSamples_;
    ic = 1.96 * sqrt(estimateur_carre/nbSamples_);
}


void MonteCarlo::price(const PnlMat *past, double t, double &prix, double &ic){
    prix = 0;
    PnlMat *path;
    double esp_carre;
    for (size_t i = 0; i < nbSamples_; ++i) {
        mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
        prix += opt_->payoff(path);
        esp_carre += pow((opt_->payoff(path)),2);
    }
    double estimateur = exp(-2*mod_->r_*opt_->T_)*((1/nbSamples_)*esp_carre-pow((1/nbSamples_)*prix,2));
    prix *= exp(-mod_->r_*opt_->T_)*(1/nbSamples_);
    ic = 1.96 * estimateur/sqrt(nbSamples_);
}

void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta, const PnlVect *conf_delta){

    delta = pnl_vect_create(mod_->size_);

    int timestep = (opt_->nbTimeSteps_+1)/opt_->T_;
    int sum = 0;
    double prix;
    double ic;
    price(past, t, prix, ic);
    double coefficient = exp(-mod_->r_ * (opt_->T_ - t))/(2 * nbSamples_ * fdStep_ * prix);

    PnlMat *path = pnl_mat_create(opt_->nbTimeSteps_ + 1, mod_->size_);
    PnlMat *increment_path = pnl_mat_create(opt_->nbTimeSteps_ + 1, mod_->size_);
    PnlMat *decrement_path = pnl_mat_create(opt_->nbTimeSteps_ + 1, mod_->size_);

    for (int d = 0; d < mod_->size_; d++) {
      for (size_t i = 0; i < nbSamples_; i++) {
        mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
        mod_->shiftAsset(increment_path, path, d, fdStep_, t, timestep);
        mod_->shiftAsset(decrement_path, path, d, -fdStep_, t, timestep);
        sum += opt_->payoff(increment_path) - opt_->payoff(decrement_path);
      }
      pnl_vect_set(delta, d, coefficient * sum);
    }
}
