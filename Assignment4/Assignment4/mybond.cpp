#include <math.h>
#include <iostream>
#include "mybond.h"

using namespace std;

bond::bond(double t0, double T0, double C0, double P0) {
	T = T0;
	C = C0;
	P = P0;
	t = t0;
}

double bond::getContinuousPrice(double r) {
	double s = 0;
	for (double i = 1; i <= T; i++) {
		s += exp(-(r)*(i - t));
	}
	return C*s + P*exp(-(r)*(T - t));
}

void bond::setPrice(double price0) {
	price = price0;

	//This algorithm looks for the YTM that will give the price.
	//The approach I used is to use two "guess" yields.
	//Adjust the yields accordingly in such a way that their differences to the price required approaches zero.
	//The yield is found if the increment between the two "guess" yields is <= 1E-9.

	//assuming these are ok starting points for r and a delta.
	double rdelta = 1.0; //increment
	double r = 0; //initial guess
	do {
		//start with getting the bond prices using rates, r and r+increment, respectively
		double rPrice = getContinuousPrice(r);
		double nextrPrice = getContinuousPrice(r + rdelta);

		//calculate the difference of the calculated prices given the input price
		double diff = rPrice - price;
		double nextdiff = nextrPrice - price;

		//cout << r << " " << nextdiff << " " << diff << " " << rdelta << endl;

		//test if the differences are of the same sign
		//if different, it means the increment overshot the approach to the price 
		//the increment has to go the other way (change of sign) with a 
		//smaller increment (divide by 10).
		if (nextdiff*diff < 0) {
			rdelta *= -1.0; // change sign of the increment
			rdelta /= 10.0; // reduce to a smaller increment
		}

		//adjust r accordingly to what would get a smaller difference
		if (fabs(nextdiff) < fabs(diff)) {
			r += rdelta;
		}
		else {
			r -= rdelta;
		}

		//check the increment limit
		if (fabs(rdelta) <= 1E-9) {
			ytm = r;
			break;
		}
	} while (true);
}

double bond::getPrice() {
	return price;
}

void bond::setYtm(double ytm0) {
	ytm = ytm0;
	price = getContinuousPrice(ytm);
}

double bond::getYtm() {
	return ytm;
}

