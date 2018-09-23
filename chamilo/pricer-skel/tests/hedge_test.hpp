//
// Created by lecalvmy on 9/20/18.
//

#ifndef MC_PRICER_TEST_PRICE_T_H
#define MC_PRICER_TEST_PRICE_T_H

#include <gtest/gtest.h>
#include "jlparser/parser.hpp"
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/**
* Programme de test pour la couverture d'un Call
*/

TEST(MonteCarlo, Call_hedge){

  const char *infile = "../../market-data/simul_call.dat";
  const PnlMat *callPath = pnl_mat_create_from_file(infile);

  double fdStep = 1; // valeur quelconque car non utilisee pour ce test

  int size = 1;
  double r = 0.02;
  double rho = 0;
  PnlVect *sigma = pnl_vect_create_from_scalar(size,0.200000);
  PnlVect *spot = pnl_vect_create_from_scalar(size,100.000000);
  PnlVect *trend = pnl_vect_create_from_zero(size);

  double T = 1.000000;
  int nbTimeSteps = 100;
  int H = 300;
  double strike = 100;

  PnlRng *rng= pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_init(rng, PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));

  size_t n_samples = 5000;

  BlackScholesModel *bsmodel = new BlackScholesModel(size, r, rho, sigma, spot, trend);
  Option *call = new AsianOption(T, nbTimeSteps, size, strike);
  PnlMat *path = pnl_mat_create(H+1, size);
  bsmodel->simul_market(path, T, rng);

  MonteCarlo *mCarlo = new MonteCarlo(bsmodel, call, rng, fdStep, n_samples);
  double pnl = 0;
  mCarlo->pnl(pnl, path, H);
  printf("P&L: %f\n", pnl);

  pnl_mat_free(&path);

  delete call;
  delete bsmodel;
  delete mCarlo;

}

#endif //MC_PRICER_TEST_PRICE_T_H
