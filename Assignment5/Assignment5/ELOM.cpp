#include "ELOM.h"
#include <cmath>
#include <iostream>
//Assignment 5: ELOM.cpp
#include <fstream>
#include <string>
#include <assert.h>
using namespace std;


//*************
//*** ORDER ***
//*************

Order::Order()
{
}

Order::~Order()
{
}

Order::Order(string NAME, int ID, int No_Shares, double Price) : m_ticker(NAME), m_id(ID), m_no_shares(No_Shares), m_price(Price)
{

}

string Order::getTicker()
{
	return m_ticker;
}

void Order::insertTicker(string NAME)
{
	m_ticker = NAME;
}

int Order::get_id()
{
	return m_id;
}

int Order::get_no_shares()
{
	return m_no_shares;
}

double Order::get_price()
{
	return m_price;
}

void Order::insertID(int ID) {
	m_id = ID;
}

void Order::insertNoShares(int NO_SHARES)
{
	m_no_shares = NO_SHARES;
}

void Order::insertPrice(double PRICE)
{
	m_price = PRICE;
}

//************
//*** ELOM ***
//************

ELOM::ELOM()
{
}

ELOM::~ELOM()
{
}

void ELOM::addOrder(Order * order, list<Order*>* listOfOrders)
{
	(*listOfOrders).push_back(order);
}

int ELOM::countOrders(list<Order*> * listOfOrders)
{
	return (*listOfOrders).size();
}



// Function to read from lists of orders (BUY and SELL)
void ELOM::readListOfOrders(string fileBuy, string fileSell)
{
	ifstream fb(fileBuy.c_str());
	ifstream fs(fileSell.c_str());

	if (fb && fs) //No need to open or close the file, it is done anyways. Verify if the streams are OK.
	{

		string line;

		//BUY
		while (getline(fb, line))
		{
			//cout << line << endl;
			Order * order = new Order(); //DO NOT delete this pointer, because it represents an element of the BUY list and we work with it therafter
			if (parseLine(line, order)) //Use of the routine parseLine
			{
				// We parse the line currently read and  add an order to the market outstanding orders
				this->addOrder(order, &m_BUY);

			}
			else
			{
				cout << "error reading line: '" << line << "'" << endl;
			}
		}

		//SELL
		while (getline(fs, line))
		{
			//cout << line << endl;
			Order * order = new Order(); //DO NOT delete this pointer, because it represents an element of the BUY list and we work with it therafter
			if (parseLine(line, order))
			{
				// We parse the line currently read and  add an order to the market outstanding orders
				this->addOrder(order, &m_SELL);
			}
			else
			{
				cout << "error reading line: '" << line << "'" << endl;
			}
		}
	}
	else
	{
		cout << "Bad file name. Execution aborted." << endl;
	}
}

// Function to output updated lists of orders (BUY and SELL)
void ELOM::writeListOfOrders(string fileBuy, string fileSell)
{
	ofstream fb(fileBuy.c_str());
	ofstream fs(fileSell.c_str());

	if (fb && fs) //No need to open or close the file, it is done anyways. Verify if the streams are OK.
	{
		//BUY
		//fb << "Ticker, TraderID, NumberOfShares, Price" << endl;   //Uncomment if header of file wanted
		cout << "BUY" << endl;
		for (list<Order*>::iterator it = m_BUY.begin(); it != m_BUY.end(); it++)
		{
			fb << (*it)->getTicker() << " " << (*it)->get_id() << " " << (*it)->get_no_shares() << " " << (*it)->get_price() << endl;
			cout << (*it)->getTicker() << " " << (*it)->get_id() << " " << (*it)->get_no_shares() << " " << (*it)->get_price() << endl;
		}

		//SELL
		//fs << "Ticker, TraderID, NumberOfShares, Price" << endl;  //Uncomment if header of file wanted
		cout << "SELL" << endl;
		for (list<Order*>::iterator it = m_SELL.begin(); it != m_SELL.end(); it++)
		{
			fs << (*it)->getTicker() << " " << (*it)->get_id() << " " << (*it)->get_no_shares() << " " << (*it)->get_price() << endl;
			cout << (*it)->getTicker() << " " << (*it)->get_id() << " " << (*it)->get_no_shares() << " " << (*it)->get_price() << endl;
		}

	}
	else
	{
		cout << "Bad file name in writeListOfOrders. Execution aborted." << endl;
	}

}


//************************
//*** Helper functions ***
//************************

bool nextToken(const string& line, int& offset, string& token)
{
	int len = line.length();
	bool rc = (offset < len);
	token = "";
	bool done = false;
	while (!done && (offset < len))
	{
		char c = line.at(offset);
		if (c == ',' || c == '\r' || c == '\n' || c == '\t' || c == ' ')
		{
			done = true;
		}
		else {
			token += c;
		}
		offset++;
	}
	return rc;
}


bool parseLine(const string& line, Order* order)
{
	int offset = 0;
	string token;

	bool rc = nextToken(line, offset, token);
	order->insertTicker(token.c_str());
	rc &= nextToken(line, offset, token);
	order->m_id = atof(token.c_str());
	rc &= nextToken(line, offset, token); //atof->int->string
	order->m_no_shares = atof(token.c_str());
	rc &= nextToken(line, offset, token);
	order->m_price = atof(token.c_str());

	return rc;
}

//Aids in sorting properly
//Just in case there is a GOOG 4.0 and GOOG 400.0
string padzeroes(string s) {
	int l = 13 - s.length();
	string ret = s;
	for (int i = 0; i <= l; i++) {
		ret = "0" + ret;
	}
	return ret;
}

//Compare passed on to list.sort(Compare comp)
//sorts by ascending order
bool compareA(Order* first, Order* second) {
	//Note: to_string - As many digits are written as needed to represent the integral part, 
	//followed by the decimal-point character and six decimal digits.

	string tickerprice_first = first->getTicker() + padzeroes(to_string(first->get_price()));
	string tickerprice_second = second->getTicker() + padzeroes(to_string(second->get_price()));
	return (tickerprice_first < tickerprice_second);
}

//Compare passed on to list.sort(Compare comp)
//sorts by descending order
bool compareD(Order* first, Order* second) {
	string tickerprice_first = first->getTicker() + padzeroes(to_string(first->get_price()));
	string tickerprice_second = second->getTicker() + padzeroes(to_string(second->get_price()));
	return (tickerprice_first > tickerprice_second);
}

int ELOM::Execute()
{
	// THIS IS THE METHOD YOU ARE ASKED TO IMPLEMENT IN ASSIGNMENT 5
	cout << "Sorting BUY list (descending)" << endl;
	m_BUY.sort(compareD);
	cout << "Sorting SELL list (ascending)" << endl;
	m_SELL.sort(compareA);

	//list everything
	cout << "BUY" << endl;
	for (list<Order*>::iterator it = m_BUY.begin(); it != m_BUY.end(); it++) {
		cout << (*it)->getTicker() << " " << (*it)->get_id() << " " << (*it)->get_no_shares() << " " << (*it)->get_price() << endl;
	}
	cout << "SELL" << endl;
	for (list<Order*>::iterator it = m_SELL.begin(); it != m_SELL.end(); it++) {
		cout << (*it)->getTicker() << " " << (*it)->get_id() << " " << (*it)->get_no_shares() << " " << (*it)->get_price() << endl;
	}
	cout << "=====================================" << endl;

	int sharestraded = 0; //count of shares traded to be returned 

	//traverse the sell list
	for (list<Order*>::iterator itSell = m_SELL.begin(); itSell != m_SELL.end(); ) {
		assert((*itSell)->get_no_shares() > 0);

		bool erasedSell = false;

		//traverse the buy list
		for (list<Order*>::iterator itBuy = m_BUY.begin(); itBuy != m_BUY.end(); ) {
			assert((*itBuy)->get_no_shares() > 0);

			bool erasedBuy = false;
			bool sellcomplete = false;

			//cout << "Comparing " << (*itBuy)->getTicker() + to_string((*itBuy)->get_price()) << " and " << (*itSell)->getTicker() + to_string((*itSell)->get_price()) << endl;

			if ((*itBuy)->getTicker() == (*itSell)->getTicker()) {
				if ((*itBuy)->get_price() >= (*itSell)->get_price()) {
					//how many shares traded in this particular match
					int shares = 0;

					//for debugging purposes
					int buyid = (*itBuy)->get_id();
					int sellid = (*itSell)->get_id();

					//implementation of the rules for trade execution
					int diff = (*itSell)->get_no_shares() - (*itBuy)->get_no_shares();
					if (diff <= 0) { //sell all
						shares = (*itSell)->get_no_shares();
						cout << shares << "\t" << (*itSell)->get_id() << " " << (*itSell)->get_no_shares() << ":" << (*itSell)->getTicker() << "@" << (*itSell)->get_price() << "\tsold to\t" << (*itBuy)->get_id() << " " << (*itBuy)->get_no_shares() << ":" << (*itBuy)->getTicker() << "@" << (*itBuy)->get_price() << endl;

						itSell = m_SELL.erase(itSell); //all SELL shares sold
						erasedSell = true; //flag whether to increment itSell iterator

						if (diff == 0) {
							itBuy = m_BUY.erase(itBuy); //all BUY shares satisfied
							erasedBuy = true; //flag whether to increment itBuy iterator
						}
						else {
							(*itBuy)->insertNoShares(-diff); //update BUY order with remaining shares not bought
						}
						sharestraded += shares;

						sellcomplete = true; //flag whether to continue the search of buyers
					}
					else { //sell some
						shares = (*itBuy)->get_no_shares();
						cout << shares << "\t" << (*itSell)->get_id() << " " << (*itSell)->get_no_shares() << ":" << (*itSell)->getTicker() << "@" << (*itSell)->get_price() << "\tsold to\t" <<  (*itBuy)->get_id() << " " << (*itBuy)->get_no_shares() << ":" << (*itBuy)->getTicker() << "@" << (*itBuy)->get_price() << endl;

						itBuy = m_BUY.erase(itBuy); //all BUY shares bought
						erasedBuy = true; //flag whether to increment itBuy iterator

						(*itSell)->insertNoShares(diff); //update SELL order with remaining shares to sell
						sharestraded += shares;
					}

					if (sellcomplete)
						break; // no need to continue traversing the buy list
				}

			}
			if (!erasedBuy) { // list.erase moves forward to the next pointer
				itBuy++;
			}
		}
		if (!erasedSell) { // list.erase moves forward to the next pointer
			itSell++;
		}
	}

	cout << "Total number of shares traded = " << sharestraded << endl;

	return sharestraded;
}
  