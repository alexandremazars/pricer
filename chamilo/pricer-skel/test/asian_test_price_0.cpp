//
// Created by teukengd on 9/14/18.
//

#include "../src/MonteCarlo.hpp"
#include "../src/BlackScholesModel.hpp"
#include "../src/Option.hpp"
#include "../src/AsianOption.hpp"

using namespace std;

int size_ = 2; /// nombre d'actifs du modèle
double r_ = 0.04879; /// taux d'intérêt
double rho_ = 0; /// paramètre de corrélation
PnlVect *sigma_ = pnl_vect_create_from_scalar(1, 0.2); /// vecteur de volatilités
PnlVect *spot_ = pnl_vect_create_from_scalar(2, 100.0); /// valeurs initiales des sous-jacents
float maturity = 1.5;
int TimeStep = 1;
double strike = 100;
double fdStep = 1;
int sampleNumber = 450;

int main(int argc, char **argv){
    BlackScholesModel *bsmodel = new BlackScholesModel(size_, r_, rho_, sigma_, spot_);
    Option *aOption = new AsianOption(maturity, TimeStep, size_, strike);
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    MonteCarlo mCarlo = new MonteCarlo(bsmodel, aOption, rng, fdStep, sampleNumber);
    double *a = null;
    cout << mCarlo.price(a , 0.1); 
}