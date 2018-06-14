#include "NLS.h"
#include "mybond.h"

//Child class inherits from FitCalculator
class TermStructure : public FitCalculator {
public:
	TermStructure(double *lambda, int n); 
	~TermStructure() {} // destructor

	//functions
	double getYield(double T);
	double getShortRate();
	double getForwardRate(double t, double S, double T);
	double getPrice(double t, double T, double C, double P);
	
	//#2e
	void fcn(int m, int n, double *lambda, double *residual, int *iflag); 

	//This is the method required in 
	//#2f
	void doMinimization(bond *bonds, int n);

private:
	double Price(bond b0);

	double *lambda;

	double beta1;
	double beta2;
	double beta3;
	double lambda2;
	double lambda3;

	//required by parent
	int nbonds;
	bond *b;
	int mParams;
};