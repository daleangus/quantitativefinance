#include "TermStructure.h"
#include <iostream>
#include <assert.h>

using namespace std;

double TermStructure::Price(bond bond) {
	double price = getPrice(bond.getTimeNow(), bond.getTimeToMaturity(), bond.getCoupon(), bond.getPrincipal());
	return price;
}

//implementation of fcn defined in parent FitCalculator
void TermStructure::fcn(int m, int n, double* lambda, double* residual, int *iflag) {
	beta1 = lambda[0];
	beta2 = lambda[1];
	beta3 = lambda[2];
	lambda2 = lambda[3];
	lambda3 = lambda[4];

	for (int i = 0; i < nbonds; i++) {
		double pHat = Price(b[i]);
		residual[i] = pHat - b[i].getPrice();
	}
}

TermStructure::TermStructure(double *lambda0, int m) {
	lambda = lambda0;

	assert(m == 5);
	beta1 = lambda0[0];
	beta2 = lambda0[1];
	beta3 = lambda0[2];
	lambda2 = lambda0[3];
	lambda3 = lambda0[4];

	mParams = m;
}

double TermStructure::getYield(double years) {

	//if years or lambda2 is equal to 0 then we know that 
	//the limit of [(1 - exp(-years*lambda))/(years*lambda2)] as years (or lambda) approaches zero is 1
	double secondTerm = 0;
	if (years == 0 || lambda2 == 0) {
		secondTerm = 1.0;
	}
	else {
		secondTerm = (1.0 - exp(-years*lambda2)) / (years*lambda2);
	}

	//same if lambda3 approaches 0
	double thirdTerm = 0;
	if (years == 0 || lambda3 == 0) {
		thirdTerm = 1.0;
	}
	else {
		thirdTerm = (1.0 - exp(-years*lambda3)) / (years*lambda3);
	}

	//compute yield
	double yield = beta1 + beta2*secondTerm + beta3*(thirdTerm - exp(-years*lambda3));
	//cout << "Y(" << years << ") = " << yield << endl;

	return yield;
}

double TermStructure::getShortRate() {
	return getYield(0);
}

double TermStructure::getForwardRate(double nowTime, double STime, double TTime) {
	assert(TTime > STime);
	double yT = getYield(TTime);
	double yS = getYield(STime);
	double fwdrate = ((TTime - nowTime)*yT - (STime - nowTime)*yS) / (TTime - STime);
	return fwdrate;
}

double TermStructure::getPrice(double nowTime, double timeToMaturity, double coupon, double principal) {
	double s = 0;
	double r = 0;
	for (double i = 1; i <= timeToMaturity; i++) {
		r = getYield(i);
		//cout << "T = " << i << ", Y(T) = " << r << endl;
		s += exp(-r*(i - nowTime));
	}
	return coupon*s + principal*exp(-(r)*(timeToMaturity - nowTime));
}

//Requires an array of bonds, the number of bonds in the array.
//Updates the lambda array provided during the instantiation.
void TermStructure::doMinimization(bond *bonds, int m) {
	b = bonds;
	nbonds = m;

	NLS nls = NLS(this, mParams, nbonds);
	nls.SOLVE(lambda);

	//update the lambda array with the tuned parameters
	lambda[0] = beta1;
	lambda[1] = beta2;
	lambda[2] = beta3;
	lambda[3] = lambda2;
	lambda[4] = lambda3;
}
