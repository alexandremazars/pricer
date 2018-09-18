//
// Created by teukengd on 9/14/18.
//

#include "../src/MonteCarlo.hpp"
#include "../src/BlackScholesModel.hpp"
#include "../src/Option.hpp"

using namespace std;

option size <int> 2;
strike <float> 100;
spot <vector> 100;
maturity <float> 1.500000;
volatility <vector> 0.2;
interest rate <float> 0.04879;
correlation <float> 0.0;
option type <string> "asian basket";
payoff coefficients <vector> 0.5;
TimeStep Number <int> 1;
Sample Number <long> 450;

int size_ = 2; /// nombre d'actifs du modèle
double r_ = 0.04879; /// taux d'intérêt
double rho_ = 0; /// paramètre de corrélation
PnlVect *sigma_ = pnl_vect_create_from_scalar(1, 0.2); /// vecteur de volatilités
PnlVect *spot_ = pnl_vect_create_from_scalar(2, 100.0); /// valeurs initiales des sous-jacents
float maturity = 1.5;
int TimeStep = 1;



int main(int argc, char **argv){
    BlackScholesModel bsmodel = new BlackScholesModel(size_, r_, rho_, sigma_, spot_);
    Option option = new Option(maturity, TimeStep)


}