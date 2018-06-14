/*
* To change this license header, choose License Headers in Project Properties.
* To change this template file, choose Tools | Templates
* and open the template in the editor.
*/

/*
* File:   main.cpp
* Author: Angus
*
* Created on January 29, 2016, 8:02 AM
*/

#include <cstdlib>
#include <iostream>
#include <math.h>

using namespace std;


double func1(double x) {
	return (x * x + 1.5 * x - 1);
}

void number1() {
	double x0 = 0;
	double x1 = 0;
	while (true) {

		cout << endl << "Enter starting point (999 ends run): ";
		cin >> x0;
		cout << endl;

		if (x0 == 999) break;

		for (int i = 1; i <= 10; i++) {

			cout << "x" << i << "= " << x0 << ", f(x" << i << ")=  " << func1(x0) << endl;
			x0 = (2.0 / 3.0) * (1.0 - (x0 * x0));
		}
	}
}

double f(double x, double a, double b, double c) {
	double fx = sin(((a * x) / (1.0 + x * x)) + 1) * atan(b * x - (1.0 / 2.0)) + exp(-c * x) * atan(x);

	return fx;
}

/**
*
* @param f is a function
* @param a
* @param b
* @param c
* @param A is less than B
* @param B
* @param N iterations
* @return
*/
double RootFinderBNew(double f(double, double, double, double), double a, double b, double c, double A, double B, int N) {
	cout << "a=" << a << " b=" << b << " c=" << c << " A=" << A << " B=" << B << " N=" << N << endl;
	double midpt = 0;
	double fmidpt = 0;

	for (int i = 1; i <= N; i++) {
		midpt = (A + B) / 2.0;
		fmidpt = f(midpt, a, b, c);

		cout << "x" << i << "= " << midpt << ", f(x" << i << ")=  " << fmidpt << endl;

		//if ((B - A) / 2.0 < 5E-8) break;

		if (f(A, a, b, c) * fmidpt > 0.0)
			A = midpt;
		else
			B = midpt;
	}
	return midpt;
}

/**
*
* @param f is a function
* @param a
* @param b
* @param c
* @param A is x sub (n - 1)
* @param B is x sub n
* @param N iterations
* @return
*/
double RootFinderSMNew(double f(double, double, double, double), double a, double b, double c, double A, double B, int N) {
	cout << "a=" << a << " b=" << b << " c=" << c << " A=" << A << " B=" << B << " N=" << N << endl;
	double x = 0;
	double fxn, fxnm1;

	for (int i = 1; i <= N; i++) {
		fxn = f(B, a, b, c);
		fxnm1 = f(A, a, b, c);

		// this is x sub (n+1)
		x = B - fxn * ((B - A) / (fxn - fxnm1));

		cout << "x" << i << "= " << x << ", f(x" << i << ")=  " << f(x, a, b, c) << endl;

		A = B;
		B = x;
	}
	return x;
}

/**
*
* @param f is a function
* @param a
* @param b
* @param c
* @param A replaces xm
* @param B replaces xp
* @param N iterations
* @return
*/
double RootFinderRidder(double f(double, double, double, double), double a, double b, double c, double A, double B, int N) {
	cout << "a=" << a << " b=" << b << " c=" << c << " A=" << A << " B=" << B << " N=" << N << endl;
	double x = 0;
	double z;
	double fz, fx;

	for (int i = 1; i <= N; i++) {
		z = (A + B) / 2.0;
		fz = f(z, a, b, c);

		x = z - (((z - A) * fz) / (sqrt(pow(fz, 2) - (f(B, a, b, c) * f(A, a, b, c)))));
		fx = f(x, a, b, c);

		cout << "x" << i << "= " << x << ", f(x" << i << ")=  " << fx << endl;

		//if (abs(A - x) < 5E-8) break;
		//if (abs(B - x) < 5E-8) break;

		if (fx < 0)
			A = x;
		else
			B = x;
	}
	return x;
}

void number2() {
	//bisection
	double a = 4.5;
	double b = 2.8;
	double c = 1.0;
	double A = -1.0;
	double B = 8.0;
	int N = 20;

	for (int i = 1; i <= 10; i++) {
		cout << endl << "Bisection method" << endl;
		RootFinderBNew(f, a, b, c, A, B, N); cout << endl;
		cout << endl << "Secant method" << endl;
		RootFinderSMNew(f, a, b, c, A, B, N); cout << endl;
		cout << endl << "Ridders' method" << endl;
		RootFinderRidder(f, a, b, c, A, B, N); cout << endl;

		cout << "Press enter... ";
		cin.get();
		A--;
		//cout << A << endl;
	}
}

/*
* Assignment 2
* Please uncomment to run the desired solution.
*/
int main(int argc, char** argv) {
	//number1();
	number2();

	return 0;
}

