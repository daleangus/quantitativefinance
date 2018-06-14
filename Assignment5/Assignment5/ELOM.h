//Assignment 5: ELOM.h
#ifndef ELOM_H
#define ELOM_H

#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <cctype>

using namespace std;

const int ticker_length=5;

class Order
{
	private:
		string m_ticker;
		int m_id;
		int m_no_shares;
		double m_price;
	public:
		friend bool parseLine(const string& line,Order* order);

		//Constructors and destructor
		Order();
		Order(string ticker, int id, int no_shares, double price);
		~Order();
		
		//Accessors
		string getTicker();
		int get_id();
		int get_no_shares();
		double get_price();
		
		//Modifiers
		void insertTicker(string NAME);		
		void insertID(int ID);
		void insertNoShares(int NO_SHARES);
		void insertPrice(double PRICE);
 };



class ELOM
{
	private:
		list<Order*> m_BUY;
		list<Order*> m_SELL;

		bool compare_by_name_price(const Order & a, const Order & b);

	public:

		//Constructors and destructor
		ELOM();
		~ELOM();

		//A method to add an order to the specified list (could be either m_BUY or m_SELL).
		//Have to set this method private thereafter since only internal use (the client should not be able to use it)
		void addOrder(Order * order, list<Order*>* listOfOrders);

		//A method to return the size of a predefined list
		int countOrders(list<Order*> * listOfOrders);

		//Read and Write method: interfaces with .txt files
		void readListOfOrders(string fileBuy, string fileSell);
		void writeListOfOrders(string fileBuy, string fileSell);

		//Trade matching and clearing function
		int Execute();
};

//prototype of helper functions in order to parse the lists of orders given as inputs
bool parseLine(const string& line,Order* order);
bool nextToken(const string& line, int& offset, string& token);


#endif