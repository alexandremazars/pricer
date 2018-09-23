#include <iostream>
#include <string>
#include "jlparser/parser.hpp"
#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "AsianOption.hpp"
#include "BasketOption.hpp"
#include "CallOption.hpp"
#include "PerformanceOption.hpp"

using namespace std;

int main(int argc, char **argv)
{
    double fdStep = 0.1;
    double T, r, strike, correlation;
    PnlVect *spot, *sigma, *divid;
    string type;
    int size, timestep;
    size_t n_samples;

    char *infile = argv[1];
    Param *P = new Parser(infile);

    P->extract("option type", type);
    P->extract("maturity", T);
    P->extract("option size", size);
    P->extract("spot", spot, size);
    P->extract("volatility", sigma, size);
    P->extract("interest rate", r);
    if (P->extract("dividend rate", divid, size, true) == false)
    {
        divid = pnl_vect_create_from_zero(size);
    }
    if (type != "performance"){
        P->extract("strike", strike);
    }
    P->extract("timestep number", timestep);    
    P->extract("sample number", n_samples);

    Option* opt;
    
    BlackScholesModel *bsmodel = new BlackScholesModel(size, r, correlation, sigma, spot);
    if (type == "asian"){
        opt = new AsianOption(T, timestep, size, strike);        
    } else if ( type == "basket"){
        opt = new BasketOption(T, timestep, size, strike);   
    } else if ( type == "performance"){
        opt = new PerformanceOption(T, timestep, size);        
    }
    PnlRng *rng= pnl_rng_create(PNL_RNG_MERSENNE);
    
    //
    pnl_rng_init(rng, PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    printf("option: %f, %i, %i\n", opt->T_, opt->nbTimeSteps_, opt->size_);
    printf("strike:%f\n", strike);
    MonteCarlo *mCarlo = new MonteCarlo(bsmodel, opt, rng, fdStep, n_samples);


    double prix = 0.0;
    double ic = 0.0;
    mCarlo->price(prix , ic);
    printf("============== \nPrix: %f \nIc: %f \n==============\n", prix, ic);

    PnlMat *past = pnl_mat_create_from_scalar(1, size, 100);
    PnlVect *delta = pnl_vect_create(size);
    PnlVect *conf_delta = pnl_vect_create(size);

    mCarlo->delta(past, 0, delta, conf_delta);
    for (int i =0; i < size; i ++){
        printf("Delta actif %u: %f\n", i+1, pnl_vect_get(delta, i));
        printf("Standard Deviation actif %u: %f\n", i+1, pnl_vect_get(conf_delta, i));
    }


    pnl_mat_free(&past);
    pnl_vect_free(&delta);
    pnl_vect_free(&conf_delta);



    delete P;

    exit(0);
}
