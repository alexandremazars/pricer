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
    double esp_carre = 0;
    path = pnl_mat_create(opt_->nbTimeSteps_ + 1, mod_->size_);
    for (size_t i = 0; i < nbSamples_; ++i) {
        mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
        prix += opt_->payoff(path);
        esp_carre += pow((opt_->payoff(path)),2);
    }
    double estimateur_carre = exp(-2*mod_->r_*opt_->T_)*(esp_carre/nbSamples_-pow(prix/nbSamples_,2));
    prix *= exp(-mod_->r_*opt_->T_)/nbSamples_;
    ic = 1.96 * sqrt(estimateur_carre/nbSamples_);
}

void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *conf_delta){

    double sum;
    double sum2;
    double timestep = opt_->T_/(opt_->nbTimeSteps_+1);
    double ic;
    double coefficient;
    double prix;

    PnlMat *path = pnl_mat_create(opt_->nbTimeSteps_ + 1, mod_->size_);
    PnlMat *increment_path = pnl_mat_create(opt_->nbTimeSteps_ + 1, mod_->size_);
    PnlMat *decrement_path = pnl_mat_create(opt_->nbTimeSteps_ + 1, mod_->size_);

    for (int d = 0; d < mod_->size_; d++) {
        sum = 0;
        sum2 = 0;
        prix = pnl_mat_get(past, past->m - 1, d);
        coefficient = exp(-mod_->r_ * (opt_->T_ - t))/(2 * fdStep_ * prix);
        for (size_t i = 0; i < nbSamples_; i++) {
            mod_->asset(path, opt_->T_, opt_->nbTimeSteps_, rng_);
            mod_->shiftAsset(increment_path, path, d, fdStep_, t, timestep);
            mod_->shiftAsset(decrement_path, path, d, -fdStep_, t, timestep);
            sum += opt_->payoff(increment_path) - opt_->payoff(decrement_path);
            sum2 += pow(opt_->payoff(increment_path) - opt_->payoff(decrement_path), 2);
        }
        pnl_vect_set(delta, d, coefficient * sum / nbSamples_);
        pnl_vect_set(conf_delta, d, sqrt((sum2/nbSamples_ - pow(sum/nbSamples_,2)) * coefficient/nbSamples_));
    }
}
