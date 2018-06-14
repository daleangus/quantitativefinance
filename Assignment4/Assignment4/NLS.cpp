#include <assert.h>
#include "NLS.h"

// The FitCalculator used by fcn() below.  The NLS method SOLVE sets (and resets)
// this variable.
FitCalculator* gblFC=NULL;

// Global implementation of the fcn function used by lmdif.
// The SOLVE method below sets the global variable gblFC (above) to 
// the current residual calculator to be used.  This function merely turns
// around and calls the fcn method of the calculator.
void fcn(int m,int n, double* lambda, double* residual, int *iflag)
{
	assert(gblFC != NULL);
	gblFC->fcn(m, n, lambda, residual, iflag);
}


NLS::NLS(FitCalculator* fc, int NumParameters,int NumDataPoints)
{
	FC = fc;  // assign the FitCalculator to be used by this solver
	n=NumParameters;  // number of parameters we're optimizing
	m=NumDataPoints;  // number of datapoints we'll use during optimization
	// LAMBDA gets populated at SOLVE time
	LAMBDA = new double[n];

	fvec=new double[m]; //no need to populate 
    ftol=1e-08; //tolerance
    xtol=1e-08; //tolerance
    gtol=1e-08; //tolerance
    maxfev=400; //maximum function evaluations
    epsfcn=1e-08; //tolerance
    diag=new double[n]; //some internal thing
    mode=1; //some internal thing
    factor=1; // a default recommended value
    nprint=0; //don't know what it does
    info=0; //output variable
    nfev=0; //output variable will store no. of function evals
    fjac=new double[m*n]; //output array of jacobian
    ldfjac=m; //recommended setting
    ipvt=new int[n]; //for internal use
    qtf=new double[n]; //for internal use
    wa1=new double[n]; //for internal use
    wa2=new double[n]; //for internal use
    wa3=new double[n]; //for internal use
    wa4=new double[m]; //for internal use
}


NLS::~NLS()
{
    delete [] fvec; 
    delete [] diag;
    delete [] fjac; 
    delete [] ipvt; 
    delete [] qtf; 
    delete [] wa1; 
    delete [] wa2; 
    delete [] wa3; 
    delete [] wa4; 
	delete [] LAMBDA;
}
void NLS::set_LAMBDA(double* lam) {
	for(int i=0;i<n;i++)
		LAMBDA[i]=lam[i];
}

void NLS::get_LAMBDA(double* lam)
{
	for(int i=0;i<n;i++)
		lam[i]=LAMBDA[i];
}

// The solver.  Sets the global residual calculator variable above, then calls lmdif
// IMPORTANT NOTE: due to the use of a global variable, this method is not threadsafe.
//
// initialParams should contain a 'good' first guess for the parameters we are solving for.
void NLS::SOLVE(const double* initialParams) 
{
	gblFC = this->FC;

	for (int i = 0; i < n; i++)
		LAMBDA[i] = initialParams[i];

	QuantLib::MINPACK::lmdif( m, n, LAMBDA, fvec, ftol,
		xtol, gtol, maxfev, epsfcn, diag,  mode,  factor,
		nprint,  &info, &nfev, fjac,ldfjac, ipvt, qtf,
		wa1, wa2, wa3, wa4);

	gblFC = NULL;

}


