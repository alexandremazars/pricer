#include <iostream>
#include <string>
#include "jlparser/parser.hpp"

using namespace std;

int main(int argc, char **argv)
{
    double T, r, strike;
    PnlVect *spot, *sigma, *divid;
    string type;
    int size;
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
    P->extract("sample number", n_samples);

    BlackScholesModel *bsmodel = new BlackScholesModel(size, r, correlation, sigma, spot);
    if (type == "asian"){
        Option *option = new AsianOption(T, timestep, size, strike);        
    } else if ( type == "basket"){
        Option *option = new BasketOption(T, timestep, size, strike);   
    } else if ( type == "performance"){
        Option *option = new PerformanceOption(T, timestep, size, strike);        
    }
    PnlRng *rng= pnl_rng_create(PNL_RNG_MERSENNE);
    //
    pnl_rng_init(rng, PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    MonteCarlo *mCarlo = new MonteCarlo(bsmodel, option, rng, fdStep, n_samples);


    delete P;

    exit(0);
}
