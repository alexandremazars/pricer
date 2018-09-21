#ifndef CHAMILO_TEST_BASKET_DELTA0
#define CHAMILO_TEST_BASKET_DELTA0
#include <gtest/gtest.h>
#include "jlparser/parser.hpp"
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"


TEST(MonteCarlo, BasketDelta0){
    double fdStep = 0.1;
    double T, r, strike, correlation;
    PnlVect *spot, *sigma, *divid;
    string type;
    int size, timestep;
    size_t n_samples;

    const char *infile = "../data/basket.dat";
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
    P->extract("strike", strike);
    P->extract("timestep number", timestep);
    P->extract("sample number", n_samples);

    BlackScholesModel *bsmodel = new BlackScholesModel(size, r, correlation, sigma, spot);
    Option *bOption = new BasketOption(T, timestep, size, strike);
    PnlRng *rng= pnl_rng_create(PNL_RNG_MERSENNE);
    //
    pnl_rng_init(rng, PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    MonteCarlo *mCarlo = new MonteCarlo(bsmodel, bOption, rng, fdStep, n_samples);

    PnlMat *past = pnl_mat_create(1, 2);
    pnl_mat_set(past, 0, 0, 100.0);
    pnl_mat_set(past, 0, 1, 100.0);
    PnlVect *delta = pnl_vect_create(2);
    PnlVect *conf_delta = pnl_vect_create(2);
    printf("delta:");
    pnl_vect_print(delta);
    printf("conf_delta:\n")   ; 
    pnl_vect_print(conf_delta);
    printf("past:") ;
    pnl_mat_print(past);

    //void delta(const PnlMat *past, double t, PnlVect *delta, const PnlVect *conf_delta);

    mCarlo->delta(past, 0, delta, conf_delta);
    ASSERT_LE(0.281640 - 0.001058, GET(conf_delta, 0)) << "Error, delta of first option not in confidence interval, too low";
    ASSERT_GE(GET(conf_delta, 0), 0.281640 + 0.001058) << "Error, delta of first option not in confidence interval, too high";
    ASSERT_LE(0.281951 - 0.001060, GET(conf_delta, 1)) << "Error, delta of second option not in confidence interval, too low";
    ASSERT_GE(GET(conf_delta, 1), 0.281951 + 0.001060) << "Error, delta of second option not in confidence interval, too high";

    pnl_mat_free(&past);
    pnl_vect_free(&delta);
    pnl_vect_free(&conf_delta);
    delete P;
}


#endif //CHAMILO_TEST_ASIAN_PRICE0
