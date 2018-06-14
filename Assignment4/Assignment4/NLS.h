#ifndef __NLS__
#define __NLS__

#include "lmdif.h"

// This function calculates the residual between exact and fitted value 
// in nonlinear least squares solver, i.e.,
// a vector: f_hat(x_i|lambda)-f(x_i), i=0...m-1 where f(x_i) are exact values
// and f_hat(x_i|lambda) are approximated values that depend on lambda
// lambda is a vector of n parameters
//
//
// Input:	m = number of function observations
//			n = number of parameters in lambda vector
//			lambda = n vector of parameters
//			xObserved = m vector of x_i values - must be defined globally
//			fObserved = m vector of f(x_i), i.e., exact values - must be defined globally
//
// Output:
//			residual = m vector of calculated residuals
//			iflag = error return variable
// Function prototype may not be changed
void fcn(int m,int n, double* lambda, double* residual, int *iflag);

// C++ facade for the lmdif calculator function.  Enables the NLS class to 'switch' between functions
//  and remain a generic least squares solver.
// The fcn method calculates residuals given parameters specified in lambda
class FitCalculator
{
public:
	virtual void fcn(int m,int n, double* lambda, double* residual, int *iflag)=0;	
};

// Interface class to MINPACK lmdif nonlinear least square solver 
class NLS 
{
public:
	NLS(FitCalculator* fc, int NumParameters=1,int NumDataPoints=1);
	~NLS();
/*	void set_function(double (*f)(double xi,double* lam));
	void set_X(double* x);
	void set_F(double* f);*/
	void set_LAMBDA(double* lam);
	void get_LAMBDA(double* lam);
	int get_n(){return n;}
	int get_m(){return m;}
	void SOLVE(const double *initialParams);
private:
	int n; // number of parameters
	int m; // number of data points we're fitting 
	double* LAMBDA; //Parameter list
    double* fvec; //no need to populate 
    double ftol; //tolerance
    double xtol; //tolerance
    double gtol; //tolerance
    int maxfev; //maximum function evaluations
    double epsfcn; //tolerance
    double* diag; //some internal thing
    int mode; //some internal thing
    double factor; // a default recommended value
    int nprint; //don't know what it does
    int info; //output variable
    int nfev; //output variable will store no. of function evals
    double* fjac; //output array of jacobian
    int ldfjac; //recommended setting
    int* ipvt; //for internal use
    double* qtf; //for internal use
    double* wa1; //for internal use
    double* wa2; //for internal use
    double* wa3; //for internal use
    double* wa4; //for internal use

	FitCalculator* FC; // provides the function that calculates residuals


};

#endif