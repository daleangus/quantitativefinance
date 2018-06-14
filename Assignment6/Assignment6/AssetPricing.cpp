#include "AssetPricing.h"
#include <assert.h>
#include <string>

using namespace std;

EuropeanPut::EuropeanPut(Stock * stock0, double strikePrice0, double timeToExpirationInYears0, double riskFreeRate0, double volatility0) {
	stock = stock0;
	strikePrice = strikePrice0;
	timeToExpirationInYears = timeToExpirationInYears0;
	riskFreeRate = riskFreeRate0;
	volatility = volatility0;
	optionType = EP;

	getValue();
}

EuropeanCall::EuropeanCall(Stock * stock0, double strikePrice0, double timeToExpirationInYears0, double riskFreeRate0, double volatility0) {
	stock = stock0;
	strikePrice = strikePrice0;
	timeToExpirationInYears = timeToExpirationInYears0;
	riskFreeRate = riskFreeRate0;
	volatility = volatility0;
	optionType = EC;

	getValue();
}

//from lecture slide
double CND(const double x) {
	const double b1 = 0.319381530;
	const double b2 = -0.356563782;
	const double b3 = 1.781477937;
	const double b4 = -1.821255978;
	const double b5 = 1.330274429;
	const double p = 0.2316419;
	const double c = 0.39894228;
	if (x >= 0.0)
	{
		double t = 1.0 / (1.0 + p * x);
		return (1.0 - c * exp(-x * x / 2.0) * t * (t *(t * (t * (t * b5 + b4) + b3) + b2) + b1));
	}
	else {
		double t = 1.0 / (1.0 - p * x);
		return (c * exp(-x * x / 2.0) * t * (t *(t * (t * (t * b5 + b4) + b3) + b2) + b1));
	}
}

double Option::BlackScholesValue(string optionType, double stockPrice, double strikePrice, double timeToExpirationInYears, double riskFreeRate, double volatility) {
	assert(optionType == EC || optionType == EP);
	assert(stockPrice > 0);
	assert(strikePrice > 0);
	assert(timeToExpirationInYears > 0);
	assert(riskFreeRate > 0);
	assert(volatility > 0);

	double d1 = (log(stockPrice / strikePrice) + (riskFreeRate + volatility* volatility / 2.0)* timeToExpirationInYears) / (volatility * sqrt(timeToExpirationInYears));

	double d2 = d1 - volatility * sqrt(timeToExpirationInYears);

	double val = 0;
	if (optionType == EC) {
		val = stockPrice*CND(d1) - strikePrice*exp(-riskFreeRate*timeToExpirationInYears)*CND(d2);
	}
	else {
		val = strikePrice*exp(-riskFreeRate*timeToExpirationInYears)*CND(-d2) - stockPrice*CND(-d1);
	}

	//cout << d1 << " " << d2 << " " << CND(d1) << " " << CND(d2) << " " << val << endl;
	return val;
}

double Option::BinomialValue(Option * option) {
	assert(option != nullptr);

	//was steps set?
	int steps = option->getSteps();
	if (steps == 0) { // not set, so use default
		steps = binomialPricingSteps;
		option->setSteps(steps);
	}
	BinomialTree bt = BinomialTree(option, steps);
	return bt.valueOption();
}

void MyAssetPricing::ValueSpots(double lowerSpotPrice, double upperSpotPrice, int numberOfSpots, char Name[]) {
	assert(option.getStock() != NULL);

	ofstream fout(Name);
	if (fout) {
		double space = (upperSpotPrice - lowerSpotPrice) / (numberOfSpots - 1);

		//put
		Option * childType = NULL;
		childType = new EuropeanPut(option.getStock(), option.getStrikePrice(), option.getTimeToExpirationInYears(), option.getRiskFreeRate(), option.getVolatility());
		for (int i = 0; i < numberOfSpots; i++) {
			double spot = lowerSpotPrice + i*space;
			childType->getStock()->setStockPrice(spot);
			cout << childType->getOptionType() << "," << childType->getStrikePrice() << "," << childType->getStock()->getValue() << "," << childType->getVolatility() << "," << childType->getTimeToExpirationInYears() << "," << childType->getRiskFreeRate() << "," << childType->getValue() << endl;
			fout << childType->getOptionType() << "," << childType->getStrikePrice() << "," << childType->getStock()->getValue() << "," << childType->getVolatility() << "," << childType->getTimeToExpirationInYears() << "," << childType->getRiskFreeRate() << "," << childType->getValue() << endl;
		}
		//call
		childType = new EuropeanCall(option.getStock(), option.getStrikePrice(), option.getTimeToExpirationInYears(), option.getRiskFreeRate(), option.getVolatility());
		for (int i = 0; i < numberOfSpots; i++) {
			double spot = lowerSpotPrice + i*space;
			childType->getStock()->setStockPrice(spot);
			cout << childType->getOptionType() << "," << childType->getStrikePrice() << "," << childType->getStock()->getValue() << "," << childType->getVolatility() << "," << childType->getTimeToExpirationInYears() << "," << childType->getRiskFreeRate() << "," << childType->getValue() << endl;
			fout << childType->getOptionType() << "," << childType->getStrikePrice() << "," << childType->getStock()->getValue() << "," << childType->getVolatility() << "," << childType->getTimeToExpirationInYears() << "," << childType->getRiskFreeRate() << "," << childType->getValue() << endl;
		}

		fout.close();
	}
	else {
		cout << "Something went wrong with the file stream. Make sure that " << Name << " is not opened by another application or disk is not full. :)" << endl;
	}
}

//Provided with assignment 5! Thank you!
bool nextToken(const string& line, int& offset, string& token) {
	int len = line.length();
	bool rc = (offset < len);
	token = "";
	bool done = false;
	while (!done && (offset < len)) {
		char c = line.at(offset);
		if (c == ',' || c == '\r' || c == '\n' || c == '\t' || c == ' ') {
			done = true;
		}
		else {
			token += c;
		}
		offset++;
	}
	return rc;
}

//Provided with assignment 5! Thank you!
//with some changes to fit requirement
bool parseLine(const string& line, Option* option) {
	int offset = 0;
	string token;

	bool rc = nextToken(line, offset, token);
	option->setOptionType(token.c_str());
	rc &= nextToken(line, offset, token);
	option->setStrikePrice(atof(token.c_str()));
	rc &= nextToken(line, offset, token);
	option->getStock()->setStockPrice(atof(token.c_str()));
	rc &= nextToken(line, offset, token);
	option->setVolatility(atof(token.c_str()));
	rc &= nextToken(line, offset, token);
	option->setTimeToExpirationInYears(atof(token.c_str()));
	rc &= nextToken(line, offset, token);
	option->setRiskFreeRate(atof(token.c_str()));

	return rc;
}

//Note the Option object passed in MyAssetPricing is not used here
void MyAssetPricing::ValueOptions(char inName[], char outName[]) {
	ifstream fin(inName);
	ofstream fout(outName);

	if (fin && fout) { //No need to open or close the file, it is done anyways. Verify if the streams are OK.
		string line;
		while (getline(fin, line)) {
			//cout << line << endl;

			Option * option = new Option(new Stock());
			if (parseLine(line, option)) {
				Option * childType = NULL;

				if (option->getOptionType() == EC)
					childType = new EuropeanCall(option->getStock(), option->getStrikePrice(), option->getTimeToExpirationInYears(), option->getRiskFreeRate(), option->getVolatility());
				else if (option->getOptionType() == EP)
					childType = new EuropeanPut(option->getStock(), option->getStrikePrice(), option->getTimeToExpirationInYears(), option->getRiskFreeRate(), option->getVolatility());
				else if (option->getOptionType() == AC)
					childType = new AmericanCall(option->getStock(), option->getStrikePrice(), option->getTimeToExpirationInYears(), option->getRiskFreeRate(), option->getVolatility());
				else if (option->getOptionType() == AP)
					childType = new AmericanPut(option->getStock(), option->getStrikePrice(), option->getTimeToExpirationInYears(), option->getRiskFreeRate(), option->getVolatility());

				cout << childType->getOptionType() << "," << childType->getStrikePrice() << "," << childType->getStock()->getValue() << "," << childType->getVolatility() << "," << childType->getTimeToExpirationInYears() << "," << childType->getRiskFreeRate() << "," << childType->getValue() << endl;
				fout << childType->getOptionType() << "," << childType->getStrikePrice() << "," << childType->getStock()->getValue() << "," << childType->getVolatility() << "," << childType->getTimeToExpirationInYears() << "," << childType->getRiskFreeRate() << "," << childType->getValue() << endl;
			}
			else {
				cout << "Something went wrong while parsing line. Make sure " << inName << " has no errors." << endl;;
			}
			delete option;
		}
		fin.close();
		fout.close();
	}
	else {
		cout << "Something went wrong with the file streams. Make sure that " << inName << " is available or the disk is not full. :)" << endl;
	}
}

double BinomialTree::valueOption() {
	assert(option != nullptr);
	bool debug = false;
	//cout << u << " " << d << " " << R << " " << getq() << " " << option->getOptionType() << endl;

	double stockPrice = option->getStock()->getValue();
	calcParams();
	double q = getq();

	//Part 2e : for put, spot < stock price
	//for call, spot > stock price
	vector<double> boundary = vector<double>(nSteps + 1);
	bool foundboundary = false;

	//at expiration	
	if (debug)
		cout << "\nAt expiration, time " << nSteps << endl;
	vector<double> C = vector<double>(nSteps + 1);
	for (int j = nSteps; j >= 0; j--) { //reverse
		double s = stockPrice * pow(u, nSteps - j) * pow(d, j);
		double payout = option->getPayout(s);
		C[j] = payout;
		if (debug)
			cout << s << '\t' << payout << endl;

		//look for exercise boundary
		if (!foundboundary) {
			if (option->getOptionType() == AP && payout == 0) {
				boundary[nSteps] = stockPrice * pow(u, nSteps - (j + 1)) * pow(d, (j + 1));
				foundboundary = true;
			}
			else if (option->getOptionType() == AC && payout > 0) {
				boundary[nSteps] = s;
				foundboundary = true;
			}
		}
	}

	//show Option prices
	if (debug) {
		for (int z = 0; z < nSteps; z++) {
			cout << C[z] << " ";
		}
		cout << endl;
	}

	//at earlier times
	vector<double> C1 = C; //preserve pricing information
	for (int i = nSteps - 1; i >= 0; i--) {
		if (debug)
			cout << "\nAt time " << i << endl;

		foundboundary = false;
		vector<double> C = vector<double>(i + 1);
		for (int j = i; j >= 0; j--) {
			double s = stockPrice * pow(u, i - j) * pow(d, j);
			double c = (q*C1[j] + (1.0 - q)*C1[j + 1]) / (1.0 + R);
			C[j] = c;
			if (debug)
				cout << i << "," << j << '\t' << s << '\t' << c << endl;

			//look for exercise boundary
			if (!foundboundary) {
				if (option->getOptionType() == AP && C[j] == 0 && C1[0] == 0) {
					boundary[i] = stockPrice * pow(u, i - (j + 1)) * pow(d, (j + 1));
					foundboundary = true;
				}
				else if (option->getOptionType() == AC && c > 0 && C1[i] == 0) {
					boundary[i] = s;
					foundboundary = true;
				}
			}
		}

		//Part 2d : Capture delta information
		if (i == 1) {
			double delta = (C[0] - C[1]) / (stockPrice*u - stockPrice*d);
			option->setDelta(delta);
			//cout << "Calculating delta " << delta << " " << C[0] << " " << C[1] << " " << stockPrice*u << " " << stockPrice*d << endl;
		}

		//show Option prices
		if (debug) {
			for (int z = 1; z < i + 1; z++) {
				cout << C[z] << " ";
			}
			cout << endl;
		}
		C1 = C; //preserve pricing information
	}

	option->setBoundary(boundary); //Part 2e

	return C1[0];
}

void BinomialTree::ValueDelta(char inName[], char outName[]) {
	ifstream fin(inName);
	ofstream fout(outName);

	if (fin && fout) {
		string line;
		while (getline(fin, line)) {
			//cout << line << endl;

			//use the Option member of Binomial tree class
			option = new Option(new Stock());
			if (parseLine(line, option)) {

				//Create the proper Option type
				//to take advantage of polymorphism, e.g., getPayOut()
				if (option->getOptionType() == EC)
					option = new EuropeanCall(option->getStock(), option->getStrikePrice(), option->getTimeToExpirationInYears(), option->getRiskFreeRate(), option->getVolatility());
				else if (option->getOptionType() == EP)
					option = new EuropeanPut(option->getStock(), option->getStrikePrice(), option->getTimeToExpirationInYears(), option->getRiskFreeRate(), option->getVolatility());
				else if (option->getOptionType() == AC)
					option = new AmericanCall(option->getStock(), option->getStrikePrice(), option->getTimeToExpirationInYears(), option->getRiskFreeRate(), option->getVolatility());
				else if (option->getOptionType() == AP)
					option = new AmericanPut(option->getStock(), option->getStrikePrice(), option->getTimeToExpirationInYears(), option->getRiskFreeRate(), option->getVolatility());

				string optionType = option->getOptionType();
				double strikePrice = option->getStrikePrice();
				double stockPrice = option->getStock()->getValue();
				double volatility = option->getVolatility();
				double timeToExpireInYears = option->getTimeToExpirationInYears();
				double riskFreeRate = option->getRiskFreeRate();
				double optionValue = valueOption();
				double delta = option->getDelta();

				cout << optionType << "," << strikePrice << "," << stockPrice << "," << volatility << "," << timeToExpireInYears << "," << riskFreeRate << "," << optionValue << "," << delta << endl;
				fout << optionType << "," << strikePrice << "," << stockPrice << "," << volatility << "," << timeToExpireInYears << "," << riskFreeRate << "," << optionValue << "," << delta << endl;
			}
			else {
				cout << "Something went wrong while parsing line. Make sure " << inName << " has no errors." << endl;;
			}
			delete option;
		}
		fin.close();
		fout.close();
	}
	else {
		cout << "Something went wrong with the file streams. Make sure that " << inName << " is available or the disk is not full. :)" << endl;
	}
}

AmericanCall::AmericanCall(Stock * stock0, double strikePrice0, double timeToExpirationInYears0, double riskFreeRate0, double volatility0) {
	stock = stock0;
	strikePrice = strikePrice0;
	timeToExpirationInYears = timeToExpirationInYears0;
	riskFreeRate = riskFreeRate0;
	volatility = volatility0;
	optionType = AC;

	getValue();
}

AmericanPut::AmericanPut(Stock * stock0, double strikePrice0, double timeToExpirationInYears0, double riskFreeRate0, double volatility0) {
	stock = stock0;
	strikePrice = strikePrice0;
	timeToExpirationInYears = timeToExpirationInYears0;
	riskFreeRate = riskFreeRate0;
	volatility = volatility0;
	optionType = AP;

	getValue();
}

