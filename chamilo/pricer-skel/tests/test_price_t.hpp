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

double T = 2.000000;
int nbTimeSteps = 365;
double strike = 100;

PnlRng *rng= pnl_rng_create(PNL_RNG_MERSENNE);

size_t n_samples = 5000;

BlackScholesModel *bsmodel = new BlackScholesModel(size, r, rho, sigma, spot);
Option *call = new CallOption(T, nbTimeSteps, size, strike);

pnl_rng_init(rng, PNL_RNG_MERSENNE);
pnl_rng_sseed(rng, time(NULL));

MonteCarlo *mCarlo = new MonteCarlo(bsmodel, call, rng, fdStep, n_samples);

double prix = 0.0;
double ic = 0.0;
double t = 0.1;
double step = floor(t * nbTimeSteps / T) + 1;
PnlMat *past = pnl_mat_create(step, size);
pnl_mat_extract_subblock(past, callPath, 0, step,  0, size);
//pnl_mat_print(past);
printf("asset t\n");
mCarlo->price(past, t, prix, ic);

PnlVect *spot2 = pnl_vect_create(size);
pnl_mat_get_row(spot2, past, (int) (past->m - 1));

double prix2 = 0;
double ic2 = 0;

BlackScholesModel *bsmodel2 = new BlackScholesModel(size, r, rho, sigma, spot2);
Option *call2  = new CallOption(T - t, nbTimeSteps, size, strike);
MonteCarlo *mCarlo2 = new MonteCarlo(bsmodel2, call2, rng, fdStep, n_samples);
printf("asset 0\n");

mCarlo2->price(prix2, ic2);
printf("prix t: %f\n",prix );
printf("prix 0: %f\n",prix2 );

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

size_t n_samples = 5000;

BlackScholesModel *bsmodel = new BlackScholesModel(size, r, rho, sigma, spot);
Option *asian = new AsianOption(T, nbTimeSteps, size, strike);

pnl_rng_init(rng, PNL_RNG_MERSENNE);
pnl_rng_sseed(rng, time(NULL));

MonteCarlo *mCarlo = new MonteCarlo(bsmodel, asian, rng, fdStep, n_samples);

double prix = 0.0;
double ic = 0.0;
double t = 0.1;
double step = floor(t * nbTimeSteps / T) + 1;
PnlMat *past = pnl_mat_create(step, size);
pnl_mat_extract_subblock(past, asianPath, 0, step,  0, size);
//pnl_mat_print(past);
mCarlo->price(past, t, prix, ic);

PnlVect *spot2 = pnl_vect_create(size);
pnl_mat_get_row(spot2, past, (int) (past->m - 1));

double prix2 = 0;
double ic2 = 0;

BlackScholesModel *bsmodel2 = new BlackScholesModel(size, r, rho, sigma, spot2);
Option *call2  = new AsianOption(T - t, nbTimeSteps, size, strike);
MonteCarlo *mCarlo2 = new MonteCarlo(bsmodel2, call2, rng, fdStep, n_samples);
mCarlo2->price(prix2, ic2);
printf("prix t: %f\n",prix );
printf("prix 0: %f\n",prix2 );

/*ASSERT_LE(prix - ic, 4.67) << "Error, price at t=0 not in confidence interval, too low";
ASSERT_GE(prix + ic, 4.67) << "Error, price at t=0 not in confidence interval, too high";
ASSERT_TRUE(ic / 1.96 >= 0.029 - 0.0001 && ic / 1.96 <= 0.29 + 0.0001);*/
}



TEST(MonteCarlo, Basket_t){

const char *infile = "../../market-data/simul_basket.dat";
const PnlMat *basketPath = pnl_mat_create_from_file(infile);

double fdStep = 1; // valeur quelconque car non utilisee pour ce test

int size = 5;
double r = 0.04879;
double rho = 0;
PnlVect *sigma = pnl_vect_create_from_scalar(size,0.200000);
PnlVect *spot = pnl_vect_create_from_scalar(size,100.000000);

double T = 1.000000;
int nbTimeSteps = 365;
double strike = 100;

PnlRng *rng= pnl_rng_create(PNL_RNG_MERSENNE);

size_t n_samples = 5000;

BlackScholesModel *bsmodel = new BlackScholesModel(size, r, rho, sigma, spot);
Option *basket = new BasketOption(T, nbTimeSteps, size, strike);

pnl_rng_init(rng, PNL_RNG_MERSENNE);
pnl_rng_sseed(rng, time(NULL));

MonteCarlo *mCarlo = new MonteCarlo(bsmodel, basket, rng, fdStep, n_samples);

double prix = 0.0;
double ic = 0.0;
double t = 0.1;
double step = floor(t * nbTimeSteps / T) + 1;
PnlMat *past = pnl_mat_create(step, size);
pnl_mat_extract_subblock(past, basketPath, 0, step,  0, size);
//pnl_mat_print(past);
mCarlo->price(past, t, prix, ic);

PnlVect *spot2 = pnl_vect_create(size);
pnl_mat_get_row(spot2, past, (int) (past->m - 1));

double prix2 = 0;
double ic2 = 0;

BlackScholesModel *bsmodel2 = new BlackScholesModel(size, r, rho, sigma, spot2);
Option *call2  = new BasketOption(T - t, nbTimeSteps, size, strike);
MonteCarlo *mCarlo2 = new MonteCarlo(bsmodel2, call2, rng, fdStep, n_samples);
mCarlo2->price(prix2, ic2);
printf("prix t: %f\n",prix );
printf("prix 0: %f\n",prix2 );
/*
ASSERT_LE(prix - ic, 4.67) << "Error, price at t=0 not in confidence interval, too low";
ASSERT_GE(prix + ic, 4.67) << "Error, price at t=0 not in confidence interval, too high";
ASSERT_TRUE(ic / 1.96 >= 0.029 - 0.0001 && ic / 1.96 <= 0.29 + 0.0001);*/
}

TEST(MonteCarlo, Basket_1_t){

const char *infile = "../../market-data/simul_basket_1.dat";
const PnlMat *basket1Path = pnl_mat_create_from_file(infile);

double fdStep = 1; // valeur quelconque car non utilisee pour ce test

int size = 40;
double r = 0.04879;
double rho = 0;
PnlVect *sigma = pnl_vect_create_from_scalar(size,0.200000);
PnlVect *spot = pnl_vect_create_from_scalar(size,100.000000);

double T = 3.000000;
int nbTimeSteps = 1000;
double strike = 100;

PnlRng *rng= pnl_rng_create(PNL_RNG_MERSENNE);

size_t n_samples = 5000;

BlackScholesModel *bsmodel = new BlackScholesModel(size, r, rho, sigma, spot);
Option *basket1 = new BasketOption(T, nbTimeSteps, size, strike);

pnl_rng_init(rng, PNL_RNG_MERSENNE);
pnl_rng_sseed(rng, time(NULL));

MonteCarlo *mCarlo = new MonteCarlo(bsmodel, basket1, rng, fdStep, n_samples);

double prix = 0.0;
double ic = 0.0;
double t = 0.1;
double step = floor(t * nbTimeSteps / T) + 1;
PnlMat *past = pnl_mat_create(step, size);
pnl_mat_extract_subblock(past, basket1Path, 0, step,  0, size);
//pnl_mat_print(past);
mCarlo->price(past, t, prix, ic);

PnlVect *spot2 = pnl_vect_create(size);
pnl_mat_get_row(spot2, past, (int) (past->m - 1));

double prix2 = 0;
double ic2 = 0;

BlackScholesModel *bsmodel2 = new BlackScholesModel(size, r, rho, sigma, spot2);
Option *call2  = new BasketOption(T - t, nbTimeSteps, size, strike);
MonteCarlo *mCarlo2 = new MonteCarlo(bsmodel2, call2, rng, fdStep, n_samples);
mCarlo2->price(prix2, ic2);
printf("prix t: %f\n",prix );
printf("prix 0: %f\n",prix2 );
/*
ASSERT_LE(prix - ic, 4.67) << "Error, price at t=0 not in confidence interval, too low";
ASSERT_GE(prix + ic, 4.67) << "Error, price at t=0 not in confidence interval, too high";
ASSERT_TRUE(ic / 1.96 >= 0.029 - 0.0001 && ic / 1.96 <= 0.29 + 0.0001);*/
}

TEST(MonteCarlo, Basket_2_t){

const char *infile = "../../market-data/simul_basket_2.dat";
const PnlMat *basket2Path = pnl_mat_create_from_file(infile);

double fdStep = 1; // valeur quelconque car non utilisee pour ce test

int size = 40;
double r = 0.04879;
double rho = 0.7;
PnlVect *sigma = pnl_vect_create_from_scalar(size,0.200000);
PnlVect *spot = pnl_vect_create_from_scalar(size,100.000000);

double T = 1.000000;
int nbTimeSteps = 365;
double strike = 100;

PnlRng *rng= pnl_rng_create(PNL_RNG_MERSENNE);

size_t n_samples = 5000;

BlackScholesModel *bsmodel = new BlackScholesModel(size, r, rho, sigma, spot);
Option *basket2 = new BasketOption(T, nbTimeSteps, size, strike);

pnl_rng_init(rng, PNL_RNG_MERSENNE);
pnl_rng_sseed(rng, time(NULL));

MonteCarlo *mCarlo = new MonteCarlo(bsmodel, basket2, rng, fdStep, n_samples);

double prix = 0.0;
double ic = 0.0;
double t = 0.1;
double step = floor(t * nbTimeSteps / T) + 1;
PnlMat *past = pnl_mat_create(step, size);
pnl_mat_extract_subblock(past, basket2Path, 0, step,  0, size);
//pnl_mat_print(past);
mCarlo->price(past, t, prix, ic);

PnlVect *spot2 = pnl_vect_create(size);
pnl_mat_get_row(spot2, past, (int) (past->m - 1));

double prix2 = 0;
double ic2 = 0;

BlackScholesModel *bsmodel2 = new BlackScholesModel(size, r, rho, sigma, spot2);
Option *call2  = new BasketOption(T - t, nbTimeSteps, size, strike);
MonteCarlo *mCarlo2 = new MonteCarlo(bsmodel2, call2, rng, fdStep, n_samples);
mCarlo2->price(prix2, ic2);
printf("prix t: %f\n",prix );
printf("prix 0: %f\n",prix2 );
/*
ASSERT_LE(prix - ic, 4.67) << "Error, price at t=0 not in confidence interval, too low";
ASSERT_GE(prix + ic, 4.67) << "Error, price at t=0 not in confidence interval, too high";
ASSERT_TRUE(ic / 1.96 >= 0.029 - 0.0001 && ic / 1.96 <= 0.29 + 0.0001);*/
}

TEST(MonteCarlo, Basket_2d_t){

const char *infile = "../../market-data/simul_basket_2d.dat";
const PnlMat *basket2dPath = pnl_mat_create_from_file(infile);

double fdStep = 1; // valeur quelconque car non utilisee pour ce test

int size = 2;
double r = 0.04879;
double rho = 0;
PnlVect *sigma = pnl_vect_create_from_scalar(size,0.200000);
PnlVect *spot = pnl_vect_create_from_scalar(size,100.000000);

double T = 1.000000;
int nbTimeSteps = 365;
double strike = 100;

PnlRng *rng= pnl_rng_create(PNL_RNG_MERSENNE);

size_t n_samples = 5000;

BlackScholesModel *bsmodel = new BlackScholesModel(size, r, rho, sigma, spot);
Option *basket2d = new BasketOption(T, nbTimeSteps, size, strike);

pnl_rng_init(rng, PNL_RNG_MERSENNE);
pnl_rng_sseed(rng, time(NULL));

MonteCarlo *mCarlo = new MonteCarlo(bsmodel, basket2d, rng, fdStep, n_samples);

double prix = 0.0;
double ic = 0.0;
double t = 0.1;
double step = floor(t * nbTimeSteps / T) + 1;
PnlMat *past = pnl_mat_create(step, size);
pnl_mat_extract_subblock(past, basket2dPath, 0, step,  0, size);
//pnl_mat_print(past);
mCarlo->price(past, t, prix, ic);

PnlVect *spot2 = pnl_vect_create(size);
pnl_mat_get_row(spot2, past, (int) (past->m - 1));

double prix2 = 0;
double ic2 = 0;

BlackScholesModel *bsmodel2 = new BlackScholesModel(size, r, rho, sigma, spot2);
Option *call2  = new BasketOption(T - t, nbTimeSteps, size, strike);
MonteCarlo *mCarlo2 = new MonteCarlo(bsmodel2, call2, rng, fdStep, n_samples);
mCarlo2->price(prix2, ic2);
printf("prix t: %f\n",prix );
printf("prix 0: %f\n",prix2 );
/*
ASSERT_LE(prix - ic, 4.67) << "Error, price at t=0 not in confidence interval, too low";
ASSERT_GE(prix + ic, 4.67) << "Error, price at t=0 not in confidence interval, too high";
ASSERT_TRUE(ic / 1.96 >= 0.029 - 0.0001 && ic / 1.96 <= 0.29 + 0.0001);*/
}

#endif //MC_PRICER_TEST_PRICE_T_H
