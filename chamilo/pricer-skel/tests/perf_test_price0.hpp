#ifndef CHAMILO_TEST_PERF_PRICE0
#define CHAMILO_TEST_PERF_PRICE0
#include <gtest/gtest.h>
#include "jlparser/parser.hpp"
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"

TEST(MonteCarlo, Performance){
    double fdStep = 1;
    double T, r, strike, correlation;
    PnlVect *spot, *sigma, *divid;
    string type;
    int size, timestep;
    size_t n_samples;

    const char *infile = "../data/perf.dat";
    Param *P = new Parser(infile);

    P->extract("option type", type);
    P->extract("maturity", T);
    P->extract("option size", size);
    P->extract("spot", spot, size);
    P->extract("correlation", correlation);
    P->extract("volatility", sigma, size);
    P->extract("interest rate", r);
    if (P->extract("dividend rate", divid, size, true) == false)
    {
        divid = pnl_vect_create_from_zero(size);
    }
    P->extract("timestep number", timestep);
    P->extract("sample number", n_samples);

    BlackScholesModel *bsmodel = new BlackScholesModel(size, r, correlation, sigma, spot);
    Option *bOption = new PerformanceOption(T, timestep, size);
    PnlRng *rng= pnl_rng_create(PNL_RNG_MERSENNE);
    //
    pnl_rng_init(rng, PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    MonteCarlo *mCarlo = new MonteCarlo(bsmodel, bOption, rng, fdStep, n_samples);
    double prix = 0.0;
    double ic = 0.0;
    mCarlo->price(prix , ic);
    ASSERT_LE(1.257353 - ic, prix) << "Error, price at t=0 not in confidence interval, too low";
    ASSERT_GE(1.257353 + ic, prix) << "Error, price at t=0 not in confidence interval, too high";
    ASSERT_TRUE(abs(ic / 1.96 - 0.000587)/0.000587 <= 0.05); // erreur relative inf a 5%
}


#endif //CHAMILO_TEST_ASIAN_PRICE0
