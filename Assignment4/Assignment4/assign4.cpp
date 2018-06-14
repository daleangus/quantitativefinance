#include <iostream>
#include "TermStructure.h"

using namespace std;

const int numParams = 5;

void run1() {
	double maturityTimeInYears = 4;
	double currentTimeInYears = 0.1;
	double couponValue = 5; // this is the actual coupon value
	double principal = 100;

	double yearToMaturity = .05; // this is not in percent
	double bondPrice = 98;

	bond bond1 = bond(currentTimeInYears, maturityTimeInYears, couponValue, principal);
	cout << "Current time = " << currentTimeInYears << endl;
	cout << "Time to maturity = " << maturityTimeInYears << endl;
	cout << "Coupon payments = " << couponValue << endl;
	cout << "Principal payment = " << principal << endl;
	bond1.setYtm(yearToMaturity);
	cout << "If YTM (continuously compounded) is " << yearToMaturity << ", bond price is " << bond1.getPrice() << endl;
	bond1.setPrice(bondPrice);
	cout << "If bond price is " << bondPrice << ", YTM (continuously compounded) is " << bond1.getYtm() << endl;

	cout << endl;
	bond1 = bond(.75, 10, 5, 100);
	cout << "Current time = " << .75 << endl;
	cout << "Time to maturity = " << 10 << endl;
	cout << "Coupon payments = " << 5 << endl;
	cout << "Principal payment = " << 100 << endl;
	bond1.setYtm(0.045);
	cout << "If YTM (continuously compounded) is " << 0.045 << ", bond price is " << bond1.getPrice() << endl;

	cout << endl;
	bond1 = bond(.5, 25, 90, 1000);
	cout << "Current time = " << .5 << endl;
	cout << "Time to maturity = " << 25 << endl;
	cout << "Coupon payments = " << 90 << endl;
	cout << "Principal payment = " << 1000 << endl;
	bond1.setPrice(1010);
	cout << "If bond price is " << 1010 << ", YTM (continuously compounded) is " << bond1.getYtm() << endl;
}

void run2() {
	double lambda[] = { .05,.05,.05,.1,2 };
	TermStructure ts = TermStructure(lambda, numParams);
	cout << "Nelson-Siegel { .05,.05,.05,.1,2 }" << endl;
	cout << "Current time = " << 0 << endl;
	cout << "Time to maturity = " << 10 << endl;
	cout << "Coupon payments = " << 5 << endl;
	cout << "Principal payment = " << 100 << endl << endl;
	cout << "Price is " << ts.getPrice(0, 10, 5, 100) << endl << endl;
	cout << "Short rate is " << ts.getShortRate() << endl;
	cout << "Forward rate F(2,3) is " << ts.getForwardRate(0, 2, 3) << endl;
	cout << "5 year zero coupon yield is " << ts.getYield(5) << endl;

	cout << endl << endl << "#2d" << endl;
	double lambda2[] = { .05,0,.2,0,1 }; // lambda2 = 0
	TermStructure ts2 = TermStructure(lambda2, numParams);
	cout << "Nelson-Siegel { .05,0,.2,0,1 }" << endl;
	cout << "Price is " << ts2.getPrice(0, 10, 5, 100) << endl;
}

void run2e() {
	const int numbonds = 10;
	double P[numbonds] = { 100,100,100,100,100,100,100,100,100,1000 };
	double C[numbonds] = { 3,3,4,5,5,6,6,6,6,90 };
	double T[numbonds] = { 1,2,15,4,5,6,7,8,9,10 }; //#2h
	double t[numbonds] = { 0,0,0,0,0,0,0,0,0,0 };
	double Price[numbonds] = { 100,100,101,101,102,102,100,100,100,1015 };
	bond bondArray[numbonds];
	for (int i = 0; i < numbonds; i++) {
		bondArray[i] = bond(t[i], T[i], C[i], P[i]);
		bondArray[i].setPrice(Price[i]);
	}

	//lambda3 with the initial Nelson-Siegel parameters 
	//it will be updated with the tuned parameters
	double lambda3[] = { 0.1,0.1,0.1,0.1,0.1 };
	TermStructure ts3 = TermStructure(lambda3, numParams); //pass lambda3 array with the initial parameters
	ts3.doMinimization(bondArray, numbonds);

	cout << "Nelson-Siegel parameters" << endl;
	for (int i = 0; i < numParams; i++) {
		cout << lambda3[i] << endl; // display the tuned lambda3 parameters
	}
	cout << endl << "Approximation of yield curve for t=0..10 years" << endl;
	for (double i = 0; i <= 10; i += .5) {
		double y = ts3.getYield(i);
		cout << i << '\t' << y << endl;
	}
}

/*
* Please uncomment to run the desired exercise
*/
int main() {

	cout << "#1" << endl;
	run1();

	cout << endl << "#2" << endl;
	run2();

	cout << endl << "#2 NLS" << endl;
	run2e();

	return 0;
}