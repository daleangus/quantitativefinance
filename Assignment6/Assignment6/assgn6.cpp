#include <iostream>
#include "AssetPricing.h"

using namespace std;

void part1() {
	Stock * stock = new Stock("ANGUS", 81);

	double strikePrice = 100;
	double volatility = 0.4;
	double timeToExpirationInYears = 1.0;
	double riskFreeRate = 0.05;

	//Test Child type
	EuropeanCall ecall = EuropeanCall(stock, strikePrice, timeToExpirationInYears, riskFreeRate, volatility);
	cout << ecall.getValue() << endl;
	EuropeanPut eput = EuropeanPut(stock, strikePrice, timeToExpirationInYears, riskFreeRate, volatility);
	cout << eput.getValue() << endl;

	//Not the preferred way to instantiate the parent Option type
	Option badOption = EuropeanCall(stock, strikePrice, timeToExpirationInYears, riskFreeRate, volatility);
	cout << badOption.getPrice() << endl; // OK
	//cout << badOption.getValue() << endl; // FAIL because explicitly not allowed 

	//Preferred way to instantiate an Option object
	Option * myOption = new EuropeanCall(stock, strikePrice, timeToExpirationInYears, riskFreeRate, volatility);
	cout << myOption->getPrice() << endl;
	cout << myOption->getValue() << endl; //polymorphism at work!

	//Part 1e
	cout << endl;
	Stock * stockz = new Stock("ZZZ", 81);
	Option option1e = Option(stockz);  //don't forget to pass the stock 
	//do not set optionType on purpose
	option1e.setStrikePrice(strikePrice);
	option1e.setTimeToExpirationInYears(timeToExpirationInYears);
	option1e.setRiskFreeRate(riskFreeRate);
	option1e.setVolatility(volatility);
	MyAssetPricing map2e = MyAssetPricing(option1e);
	double lowerSpotPrice = 75;
	double upperSpotPrice = 125;
	int numberOfSpots = 51;
	char filename[] = "ValueSpots.csv";
	map2e.ValueSpots(lowerSpotPrice, upperSpotPrice, numberOfSpots, filename);
	map2e.ValueOptions("ValueOptionsInput.csv", "ValueOptionsOutput.csv");
}

void part2() {
	Stock * stock = new Stock("ANGUS", 80);
	Option * option = new EuropeanCall(stock, 90, 1.0, .1, .25);
	BinomialTree bt = BinomialTree(option, 20);
	cout << "Test Option value " << bt.valueOption() << " (" << bt.getSteps() << " steps)" << endl << endl;

	Stock * stock2b = new Stock("XYZ", 90);
	Option * option2b = new EuropeanCall(stock2b, 90, .5, .08, .25);
	double optionValue = option2b->getPrice();

	int steps = 2; //start with 2 steps
	do {
		BinomialTree bt2b = BinomialTree(option2b, steps);
		double valueOption = bt2b.valueOption();
		cout << steps << " steps " << optionValue << " " << valueOption << " (" << bt2b.getSteps() << " steps)" << endl;
		if (fabs(optionValue - valueOption) <= .01) { //within a penny
			break;
		}
		steps++;
	} while (true);
	cout << "Part 2b: Tree depth = " << steps << endl;
}

void part2c() {
	Stock * stock2c = new Stock("AMER", 100);
	double X2c = 110.0;
	Option * aput = new AmericanPut(stock2c, X2c, 2, .06, .3);

	//do not do this!!! 
	//cout << "Test Binomial Pricing of American Put " << aput->getValue() << " (" << aput->getSteps() << " steps)" << endl;
	//getSteps() return 0. Maybe cout created two different copies of aput?

	double ovalue = aput->getValue();
	cout << "Test Binomial Pricing of American Put " << ovalue << " (" << aput->getSteps() << " steps)" << endl;
	Option * acall = new AmericanCall(stock2c, X2c, 2, .06, .3);
	ovalue = acall->getValue();
	cout << "Test Binomial Pricing of American Call " << ovalue << " (" << acall->getSteps() << " steps)" << endl << endl;

	ofstream fout("PartIIc.csv");
	double maxdiffprice = 0;
	double maxdiff = 0;
	cout << "Price" << '\t' << "European" << '\t' << "American" << '\t' << "abs diff" << endl;
	for (int i = 80; i <= 110; i++) {

		AmericanPut * aput = new AmericanPut(stock2c, i, 2, .06, .3); //child type
		aput->setSteps(200); //override the default number of steps

		Option * eput = new EuropeanPut(stock2c, i, 2, .06, .3); //or parent type (nice!)

		double diff = fabs(eput->getValue() - aput->getValue());
		cout << i << '\t' << eput->getValue() << "\t\t" << aput->getValue() << "\t\t" << diff << " (" << aput->getSteps() << " steps)" << endl;
		fout << i << "," << eput->getValue() << "," << aput->getValue() << endl;
		if (diff > maxdiff) {
			maxdiff = diff;
			maxdiffprice = i;
		}
		delete aput;
		delete eput;
	}
	cout << "Price " << maxdiffprice << " has largest difference, " << maxdiff << ". Stock price is " << stock2c->getValue() << ". Hmmmm" << endl;
	fout.close();
}

void part2d() {
	Stock * stock2d = new Stock("XYZ", 90);
	Option * option2d = new AmericanCall(stock2d, 90, 1, .08, .25);
	double p2dodel = option2d->getDelta();
	cout << "Test getDelta " << p2dodel << endl;

	//only American options return delta. Did not implement for European options.
	Option * eoption2d = new EuropeanCall(stock2d, 90, 1, .08, .25);
	double checkeo = eoption2d->getDelta(); //will return 0
	cout << "Test getDelta " << checkeo << endl;

	cout << endl << "Part IId : Approximate Delta" << endl;
	BinomialTree bt2d = BinomialTree(new Option(new Stock()), 20);
	bt2d.ValueDelta("ValueDeltaInput.csv", "ValueDeltaOutput.csv");
}

void part2e() {
	//only American option would return boundary calculation
	int steps = 100;

	Stock * stock2e = new Stock("COCO", 90);

	//Part 2e #1
	Option * option2e = new AmericanPut(stock2e, 100, 1, .05, .2);
	BinomialTree bt2e = BinomialTree(option2e, steps);
	bt2e.valueOption();
	vector<double> bndry = option2e->getBoundary();

	//Part 2e #2
	Option * option2e2 = new AmericanPut(stock2e, 100, 1, .05, .4);
	BinomialTree bt2e2 = BinomialTree(option2e2, steps);
	bt2e2.valueOption();
	vector<double> bndry2 = option2e2->getBoundary();

	//Display side by side
	cout << endl << "Part 2e : Exercise Boundary" << endl << "Steps\tVolatility=.2\tVolatility=.4" << endl;
	for (int i = 0; i < bndry.size(); i++)
		cout << i << "\t" << (bndry[i] == 0 ? "NONE      " : to_string(bndry[i])) << "\t" << (bndry2[i] == 0 ? "NONE      " : to_string(bndry2[i])) << endl;

	cout << endl;
}

void part3a() {
	cout << "Part III: Chooser Option" << endl;

	ofstream fout("Part3a.csv");

	Stock * stock = new Stock("STOK", 100);

	Option * ecall = new EuropeanCall(stock, 85, 1, .02, .45);
	Option * eput = new EuropeanPut(stock, 85, 1, .02, .45);

	Option * acall = new AmericanCall(stock, 85, 1, .02, .45);
	Option * aput = new AmericanPut(stock, 85, 1, .02, .45);

	double decisionTime = 0;
	ChooserOption europeanChooser = ChooserOption(ecall, eput, decisionTime);
	ChooserOption americanChooser = ChooserOption(acall, aput, decisionTime);

	cout << "Stock Price:\t" << stock->getValue() << endl;
	cout << "\t\tCall\tPut" << endl;
	cout << "Option Type:\t" << acall->getOptionType() << "\t" << aput->getOptionType() << endl;
	cout << "Strike Price:\t(changes from 85 to 115)" << endl;
	cout << "Time To Expire:\t" << acall->getTimeToExpirationInYears() << "\t" << aput->getTimeToExpirationInYears() << endl;
	cout << "Risk-free Rate:\t" << acall->getRiskFreeRate() << "\t" << aput->getRiskFreeRate() << endl;
	cout << "Volatility:\t" << acall->getVolatility() << "\t" << aput->getVolatility() << endl;
	cout << endl;
	cout << "Option Type:\t" << ecall->getOptionType() << "\t" << eput->getOptionType() << endl;
	cout << "Strike Price:\t(changes from 85 to 115)" << endl;
	cout << "Time To Expire:\t" << ecall->getTimeToExpirationInYears() << "\t" << eput->getTimeToExpirationInYears() << endl;
	cout << "Risk-free Rate:\t" << ecall->getRiskFreeRate() << "\t" << eput->getRiskFreeRate() << endl;
	cout << "Volatility:\t" << ecall->getVolatility() << "\t" << eput->getVolatility() << endl;
	cout << endl;
	//Part 3a
	cout << "Strike" << "\t" << "European" << "\t" << "American" << "\t" << "diff" << endl;
	//change the strike price of underlying vanilla options from 85 to 115
	for (double d = 85; d <= 115; d++) {
		europeanChooser.setCallStrike(d);
		europeanChooser.setPutStrike(d);
		americanChooser.setCallStrike(d);
		americanChooser.setPutStrike(d);
		//can change the strike price directly via the option pointers above but I like it this way.

		double diff = americanChooser.getPrice() - europeanChooser.getPrice();
		cout << d << "\t" << europeanChooser.getPrice() << "\t\t" << americanChooser.getPrice() << "\t\t" << diff << endl;
		fout << d << "," << europeanChooser.getPrice() << "," << americanChooser.getPrice() <<  endl;
	}
	cout << endl;
}

void part3b() {
	ofstream  fout("Part3b.csv");

	Stock * stock = new Stock("STOK", 100);

	Option * acall = new AmericanCall(stock, 110, 2, .02, .45);
	Option * aput = new AmericanPut(stock, 90, 2, .02, .45); 

	double decisionTime = 0;
	ChooserOption chooser = ChooserOption(acall, aput, decisionTime);

	cout << "Stock Price:\t(changes from 50 to 150)" << endl;
	cout << "\t\tCall\tPut" << endl;
	cout << "Option Type:\t" << acall->getOptionType() << "\t" << aput->getOptionType() << endl;
	cout << "Strike Price:\t" << acall->getStrikePrice() << "\t" << aput->getStrikePrice() << endl;
	cout << "Time To Expire:\t" << acall->getTimeToExpirationInYears() << "\t" << aput->getTimeToExpirationInYears() << endl;
	cout << "Risk-free Rate:\t" << acall->getRiskFreeRate() << "\t" << aput->getRiskFreeRate() << endl;
	cout << "Volatility:\t" << acall->getVolatility() << "\t" << aput->getVolatility() << endl;
	cout << "Decision Time:\t" << decisionTime << " years" << endl << endl;
	//Part 3b
	cout << "Spot\tChooser\t\tCall+Put\tCall\t\tPut" << endl;
	//change stock price of underlying vanilla options from 50 to 150
	for (double d = 50; d <= 150; d++) {
		chooser.setSpot(d);
		//can change the underlying stock price via the stock pointer above but I like it this way.

		double chooserPrice = chooser.getPrice();
		double acallPrice = acall->getPrice();
		double aputPrice = aput->getPrice();
		cout << d << "\t" << chooserPrice << "\t\t" << acallPrice + aputPrice << "\t\t" << acallPrice << "\t\t" << aputPrice << endl;
		fout << d << "," << chooserPrice << "," << acallPrice + aputPrice << "," << acallPrice << "," << aputPrice << endl;
	}
}

/* Dale Angus
Please uncomment to run desired exercise
*/
int main() {
	part1();

	part2();
	part2c();
	part2d();
	part2e();

	part3a(); //outputs to part3a.csv
	part3b(); //outputs to part3b.csv

	return 0;
}