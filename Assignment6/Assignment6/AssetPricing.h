#ifndef __AssetPricing__
#define __AssetPricing__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assert.h>

using namespace std;

//const double PI = 4.0*atan(1.0);
const string EC = "EC";
const string EP = "EP";
const string AC = "AC";
const string AP = "AP";
const int binomialPricingSteps = 20;

class Asset {
public:
	//constructor and destructor
	Asset() {}
	~Asset() {}

	virtual double getValue() = 0;
};

class Stock : public Asset {
public:
	//constructors and destructor
	Stock() {}
	Stock(string ticker0, double stockPrice0) {
		ticker = ticker0;
		stockPrice = stockPrice0;
	}
	~Stock() {}

	//setters
	void setTicker(string ticker0) { ticker = ticker0; }
	void setStockPrice(double stockPrice0) { stockPrice = stockPrice0; }

	//getters
	string getTicker() { return ticker; }
	double getValue() { return stockPrice; }

private:
	string ticker;
	double stockPrice;
};

class Option : public Asset {
	friend class MyAssetPricing;

protected:
	//hid this constructor to prevent 
	//	Option * myOption = new Option();
	Option() {}

public:
	//constructor and destructor
	Option(Stock * stock0) { stock = stock0; }
	~Option() {}

	double BlackScholesValue(string optionType, double stockPrice, double strikePrice, double timeToExpirationInYears, double riskFreeRate, double volatility);
	double BinomialValue(Option * option);

	//polymorphism!!!!
	virtual double getPayout(double spot) {
		assert(1 == 0); //force to fail!
		return 0;
	}

	//polymorphism!!!!
	virtual double getValue() {
		assert(1 == 0); //force to fail!
		return 0;
	}

	//setters
	void setStock(Stock * stock0) { stock = stock0; }
	void setStrikePrice(double strikePrice0) { strikePrice = strikePrice0; }
	void setTimeToExpirationInYears(double timeToExpirationInYears0) { timeToExpirationInYears = timeToExpirationInYears0; }
	void setRiskFreeRate(double riskFreeRate0) { riskFreeRate = riskFreeRate0; }
	void setVolatility(double volatility0) { volatility = volatility0; }
	void setOptionType(string optionType0) { optionType = optionType0; }
	void setSteps(int nSteps0) { nSteps = nSteps0; }
	void setDelta(double delta0) { delta = delta0; }
	void setBoundary(vector<double> boundary0) { boundary = boundary0; }

	//getters
	double getStrikePrice() { return strikePrice; }
	double getTimeToExpirationInYears() { return timeToExpirationInYears; }
	double getRiskFreeRate() { return riskFreeRate; }
	double getVolatility() { return volatility; }
	string getOptionType() { return optionType; }
	Stock * getStock() { return stock; }
	double getDelta() { return delta; } // Only American options will return a valid delta
	double getPrice() { return price; }
	vector<double> getBoundary() { return boundary; }

	int getSteps() { return nSteps; } // only American options

protected:
	Stock * stock;
	double strikePrice;
	double timeToExpirationInYears;
	double riskFreeRate;
	double volatility;
	string optionType;
	Option * option = this;
	double price;
	double delta;
	vector<double> boundary;

	int nSteps; //binomial pricing steps. Used by American options

};

class AmericanCall : public Option {
public:
	//constructor and destructor
	AmericanCall(Stock * stock, double strikePrice, double timeToExpirationInYears, double riskFreeRate, double volatility);
	~AmericanCall() {}

	double getValue() {
		price = BinomialValue(option);
		return price;
	}

	//polymorphism!!!!
	virtual double getPayout(double spot) {
		double ret = 0;
		if (spot - strikePrice > 0)
			ret = spot - strikePrice;
		return ret;
	}
};

class AmericanPut :public Option {
public:
	//constructor and destructor
	AmericanPut(Stock * stock, double strikePrice, double timeToExpirationInYears, double riskFreeRate, double volatility);
	~AmericanPut() {}

	double getValue() {
		price = BinomialValue(option);
		return price;
	}

	//polymorphism!!!!
	virtual double getPayout(double spot) {
		double ret = 0;
		if (strikePrice - spot > 0)
			ret = strikePrice - spot;
		return ret;
	}
};

class EuropeanCall : public Option {
public:
	//constructor and destructor
	EuropeanCall(Stock * stock, double strikePrice, double timeToExpirationInYears, double riskFreeRate, double volatility);
	~EuropeanCall() {}

	double getValue() {
		price = BlackScholesValue(optionType, stock->getValue(), strikePrice, timeToExpirationInYears, riskFreeRate, volatility);
		return price;
	}

	//polymorphism!!!!
	virtual double getPayout(double spot) {
		double ret = 0;
		if (spot - strikePrice > 0)
			ret = spot - strikePrice;
		return ret;
	}
};

class EuropeanPut :public Option {
public:
	//constructor and destructor
	EuropeanPut(Stock * stock, double strikePrice, double timeToExpirationInYears, double riskFreeRate, double volatility);
	~EuropeanPut() {}

	double getValue() {
		price = BlackScholesValue(optionType, stock->getValue(), strikePrice, timeToExpirationInYears, riskFreeRate, volatility);
		return price;
	}

	//polymorphism!!!!
	virtual double getPayout(double spot) {
		double ret = 0;
		if (strikePrice - spot > 0)
			ret = strikePrice - spot;
		return ret;
	}
};

class MyAssetPricing {
public:
	//constructor and destructor
	MyAssetPricing(Option option0) { option = option0; }
	~MyAssetPricing() {}

	void ValueOptions(char inName[], char outName[]);
	void ValueSpots(double lowerSpotPrice, double upperSpotPrice, int numberOfSpots, char Name[]);

private:
	Option option;
};

class BinomialTree {
public:
	//constructor and destructor
	BinomialTree(Option * option0, int nSteps0) {
		option = option0;
		nSteps = nSteps0;
		calcParams();
	}
	~BinomialTree() {}

	double valueOption();

	void ValueDelta(char inName[], char outName[]); //Part 2d

	//getters
	int getSteps() { return nSteps; }
	double getq() { return (1 + R - d) / (u - d); }

private:
	Option * option;

	int nSteps; //defaults to 20, see binomialPricingSteps constant

	//parameters
	double u;
	double d;
	double R;

	BinomialTree() {} // hide this constructor

	//Everytime the member *option changes, update the parameters
	//Called by constructor, methods valueOption and ValueDelta
	void calcParams() {
		double volatility = option->getVolatility();
		double riskFreeRate = option->getRiskFreeRate();

		double deltaT = option->getTimeToExpirationInYears() / nSteps;
		d = exp(-volatility *sqrt(deltaT));
		u = exp(volatility *sqrt(deltaT));
		R = exp(riskFreeRate*deltaT) - 1;
	}
};

//Part 3
class ChooserOption {
public:
	ChooserOption(Option * callOption0, Option * putOption0, double decisionTime0) {
		assert(callOption0->getOptionType() == AC || callOption0->getOptionType() == EC); //must be a call option
		assert(putOption0->getOptionType() == AP || putOption0->getOptionType() == EP); //must be a put option

		//decistionTime should not be negative and should be less than the expiration time of both call and put options
		assert(decisionTime0 >= 0 && decisionTime0 <= callOption0->getTimeToExpirationInYears() && decisionTime0 <= putOption0->getTimeToExpirationInYears());

		callOption = callOption0;
		putOption = putOption0;
		decisionTime = decisionTime0;
	}
	~ChooserOption() {}

	void setCallStrike(double strikePrice) {
		callOption->setStrikePrice(strikePrice);
		callOption->getValue();
	}

	void setPutStrike(double strikePrice) {
		putOption->setStrikePrice(strikePrice);
		putOption->getValue();
	}

	double getPrice() {
		Option * c;
		Option * p;
		
		if (callOption->getOptionType() == EC)
			c = new EuropeanCall(callOption->getStock(), callOption->getStrikePrice(), callOption->getTimeToExpirationInYears() - decisionTime, callOption->getRiskFreeRate(), callOption->getVolatility());
		else 
			c = new AmericanCall(callOption->getStock(), callOption->getStrikePrice(), callOption->getTimeToExpirationInYears() - decisionTime, callOption->getRiskFreeRate(), callOption->getVolatility());

		if (putOption->getOptionType() == EP)
			p = new EuropeanPut(putOption->getStock(), putOption->getStrikePrice(), putOption->getTimeToExpirationInYears() - decisionTime, putOption->getRiskFreeRate(), putOption->getVolatility());
		else
			p = new AmericanPut(putOption->getStock(), putOption->getStrikePrice(), putOption->getTimeToExpirationInYears() - decisionTime, putOption->getRiskFreeRate(), putOption->getVolatility());

		double price = c->getPrice() + p->getPrice();

		return price;
	}

	void setSpot(double stockPrice) {
		Stock * stock = callOption->getStock(); //use the stock from the call option. Both options point to the same stock pointer.
		assert(callOption->getStock() == putOption->getStock()); //just to be sure :)

		stock->setStockPrice(stockPrice);
		callOption->getValue();
		putOption->getValue();
	}

private:
	Option * callOption;
	Option * putOption;
	double decisionTime;
};
#endif