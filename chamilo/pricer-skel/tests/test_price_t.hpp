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

TEST(MonteCarlo, Call_t){

const char *infile = "../../market-data/simul_call.dat";
const PnlMat *callPath = pnl_mat_create_from_file(infile);

double fdStep = 1; // valeur quelconque car non utilisee pour ce test

int size = 1;
double r = 0.02;
double rho = 0;
PnlVect *sigma = pnl_vect_create_from_scalar(size,0.200000);
PnlVect *spot = pnl_vect_create_from_scalar(size,100.000000);

double T = 1.000000;
int nbTimeSteps = 365;
double strike = 100;

PnlRng *rng= pnl_rng_create(PNL_RNG_MERSENNE);

size_t n_samples = 50000;

BlackScholesModel *bsmodel = new BlackScholesModel(size, r, rho, sigma, spot);
Option *call = new CallOption(T, nbTimeSteps, size, strike);

pnl_rng_init(rng, PNL_RNG_MERSENNE);
pnl_rng_sseed(rng, time(NULL));

MonteCarlo *mCarlo = new MonteCarlo(bsmodel, call, rng, fdStep, n_samples);

double prix = 0.0;
double ic = 0.0;
double t = 30;
PnlMat *past = pnl_mat_create(t, size);
pnl_mat_extract_subblock(past, callPath, 0, t+1, 0, size);

//mCarlo->price(past, t, prix, ic);
/*
ASSERT_LE(prix - ic, 4.67) << "Error, price at t=0 not in confidence interval, too low";
ASSERT_GE(prix + ic, 4.67) << "Error, price at t=0 not in confidence interval, too high";
ASSERT_TRUE(ic / 1.96 >= 0.029 - 0.0001 && ic / 1.96 <= 0.29 + 0.0001);*/
}

TEST(MonteCarlo, Asian_t){

const char *infile = "../../market-data/simul_asian.dat";
const PnlMat *asianPath = pnl_mat_create_from_file(infile);

double fdStep = 1; // valeur quelconque car non utilisee pour ce test

int size = 2;
double r = 0.02;
double rho = 0;
PnlVect *sigma = pnl_vect_create_from_scalar(size,0.200000);
PnlVect *spot = pnl_vect_create_from_scalar(size,100.000000);

double T = 1.500000;
int nbTimeSteps = 450;
double strike = 90;

PnlRng *rng= pnl_rng_create(PNL_RNG_MERSENNE);

size_t n_samples = 50000;

BlackScholesModel *bsmodel = new BlackScholesModel(size, r, rho, sigma, spot);
Option *asian = new AsianOption(T, nbTimeSteps, size, strike);

pnl_rng_init(rng, PNL_RNG_MERSENNE);
pnl_rng_sseed(rng, time(NULL));

MonteCarlo *mCarlo = new MonteCarlo(bsmodel, asian, rng, fdStep, n_samples);

double prix = 0.0;
double ic = 0.0;
double t = 50;
PnlMat *past = pnl_mat_create(t, size);
pnl_mat_extract_subblock(past, asianPath, 0, t+1, 0, size);

//mCarlo->price(past, t, prix, ic);
/*
ASSERT_LE(prix - ic, 4.67) << "Error, price at t=0 not in confidence interval, too low";
ASSERT_GE(prix + ic, 4.67) << "Error, price at t=0 not in confidence interval, too high";
ASSERT_TRUE(ic / 1.96 >= 0.029 - 0.0001 && ic / 1.96 <= 0.29 + 0.0001);*/
}


#endif //MC_PRICER_TEST_PRICE_T_H
