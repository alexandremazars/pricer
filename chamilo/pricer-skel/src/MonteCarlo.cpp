//
// Created by lecalvmy on 9/13/18.
//

#include "MonteCarlo.hpp"

using namespace std;

/**
* Constructeur de la classe
* param[in]
* BlackScholesModel *mod : pointeur vers le modèle
* Option *opt : pointeur sur l'option
* PnlRng *rng : pointeur sur le générateur
* double fdStep : pas de différence finie
* size_t nbSamples : nombre de tirages Monte Carlo
*/
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
    double payoff;
    prix = 0;
    PnlMat *path;
    double esp_carre = 0; //premier membre pour calculer la variance
    path = pnl_mat_create(opt_->nbTimeSteps_ + 1, mod_->size_);
    for (size_t j = 0; j < nbSamples_; ++j) {
        mod_->asset(path, opt_->T_, opt_->nbTimeSteps_, rng_);
        payoff = opt_->payoff(path);
        prix += payoff;
        esp_carre += pow(payoff,2);
    }
    double estimateur_carre = exp(-2*mod_->r_*opt_->T_)*(esp_carre/nbSamples_-pow(prix/nbSamples_,2));
    prix *= exp(-mod_->r_*opt_->T_)/nbSamples_;
    ic = 1.96 * sqrt(estimateur_carre/nbSamples_);

    pnl_mat_free(&path);
}


void MonteCarlo::price(const PnlMat *past, double t, double &prix, double &ic){
    double payoff;
    prix = 0;
    PnlMat *path;
    double esp_carre = 0;
    path = pnl_mat_create(opt_->nbTimeSteps_ + 1, mod_->size_);
    for (size_t i = 0; i < nbSamples_; ++i) {
        mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
        payoff = opt_->payoff(path);
        prix += payoff;
        esp_carre += pow(payoff,2);
    }
    double estimateur_carre = exp(-2*mod_->r_*opt_->T_)*(esp_carre/nbSamples_-pow(prix/nbSamples_,2));
    prix *= exp(-mod_->r_*opt_->T_)/nbSamples_;
    ic = 1.96 * sqrt(estimateur_carre/nbSamples_);

    pnl_mat_free(&path);
}

void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *conf_delta){

    double sum;
    double sum2;
    double timestep = opt_->T_/(opt_->nbTimeSteps_+1);
    double ic;
    double coefficient;
    double prix;
    double payoff_increment;
    double payoff_decrement;

    PnlMat *path = pnl_mat_create(opt_->nbTimeSteps_ + 1, mod_->size_);
    PnlMat *increment_path = pnl_mat_create(opt_->nbTimeSteps_ + 1, mod_->size_);
    PnlMat *decrement_path = pnl_mat_create(opt_->nbTimeSteps_ + 1, mod_->size_);

    for (int d = 0; d < mod_->size_; d++) {
        sum = 0;
        sum2 = 0;
        prix = pnl_mat_get(past, past->m - 1, d);
        coefficient = exp(-mod_->r_ * (opt_->T_ - t))/(2 * fdStep_ * prix);
        for (size_t i = 0; i < nbSamples_; i++) {
            mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
            mod_->shiftAsset(increment_path, path, d, fdStep_, t, timestep);
            mod_->shiftAsset(decrement_path, path, d, -fdStep_, t, timestep);
            payoff_increment = opt_->payoff(increment_path);
            payoff_decrement = opt_->payoff(decrement_path);
            sum += payoff_increment - payoff_decrement;
            sum2 += pow(payoff_increment - payoff_decrement, 2);
        }
        pnl_vect_set(delta, d, coefficient * sum / nbSamples_);
        double standard_dev = coefficient * sqrt(sum2 / nbSamples_ - pow(sum/nbSamples_, 2));
        pnl_vect_set(conf_delta, d, standard_dev / sqrt(nbSamples_));
    }

    pnl_mat_free(&path);
    pnl_mat_free(&increment_path);
    pnl_mat_free(&decrement_path);
}


void MonteCarlo::listPrice(PnlVect *listPrice, PnlMat *marketPrice, int H, int size){
        double delta_h = (opt_->T_)/H;
        double price_h = 0;
        double ic = 0;
        PnlMat *past = pnl_mat_create(1, size);
        PnlVect *value_h = pnl_vect_create(size);
        pnl_mat_extract_subblock(past, marketPrice, 0, 1,  0, size);
        for (int i = 0; i < H + 1; i++) {
                if (i == 0) {
                        price(price_h, ic);
                        pnl_vect_set(listPrice, price_h, i);
                }
                else{
                        pnl_mat_get_row(value_h, marketPrice, i);
                        pnl_mat_add_row(past, i, value_h);
                        price(past,i*delta_h, price_h, ic);
                        pnl_vect_set(listPrice, price_h, i);
                }
        }
        pnl_vect_free(&value_h);
        pnl_mat_free(&past);
}

void MonteCarlo::matDelta(PnlMat *matDelta,PnlMat *marketPrice, int H, int size){
        double delta_h = (opt_->T_)/H;
        double price_h = 0;
        double ic = 0;
        PnlMat *past = pnl_mat_create(0,size);
        PnlVect *value_h = pnl_vect_create(size);
        PnlVect *listdelta = pnl_vect_create(size);
        PnlVect *conf_delta = pnl_vect_create(size);
        for (int i = 0; i < H + 1; i++) {
                pnl_mat_get_row(value_h, marketPrice, i);
                pnl_mat_add_row(past, i, value_h);
                delta(past, i*delta_h,listdelta, conf_delta);
                pnl_mat_set_row(matDelta, listdelta, i);
        }
        pnl_mat_free(&past);
        pnl_vect_free(&value_h);
        pnl_vect_free(&listdelta);
        pnl_vect_free(&conf_delta);
}

void MonteCarlo::listHedge(PnlVect *listHedge,PnlMat *marketPrice){
        double H = marketPrice->m - 1;
        int size = marketPrice->n;
        PnlVect *price = pnl_vect_create(H+1);
        PnlMat *deltas = pnl_mat_create(H+1,size);
        listPrice(price, marketPrice, H, size);
        matDelta(deltas, marketPrice, H, size);
        double price_h = pnl_vect_get(price, 0);
        PnlVect *deltaSize_prec = pnl_vect_create(size);
        PnlVect *deltaSize = pnl_vect_create(size);
        PnlVect *valueSize = pnl_vect_create(size);
        price_h = pnl_vect_get(price, 0);
        pnl_mat_get_row(deltaSize_prec, deltas, 0);
        pnl_mat_get_row(valueSize,marketPrice, 0);
        double hedgeValue ;
        hedgeValue = price_h - pnl_vect_scalar_prod(deltaSize_prec, valueSize);
        for (int i = 1; i < H+1; i++) {
          pnl_mat_get_row(deltaSize, deltas, i);
          pnl_mat_get_row(valueSize,marketPrice, i);
          pnl_vect_minus_vect(deltaSize_prec, deltaSize);
          pnl_vect_minus(deltaSize_prec);
          hedgeValue = hedgeValue * exp(mod_->r_*opt_->T_/H)*pnl_vect_scalar_prod(deltaSize_prec, valueSize);
          deltaSize_prec = deltaSize;
        }
        pnl_vect_free(&price);
        pnl_mat_free(&deltas);
        pnl_vect_free(&deltaSize_prec);
        pnl_vect_free(&deltaSize);
        pnl_vect_free(&valueSize);
}
