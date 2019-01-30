#pragma once
#include <iostream>
#include <string>
#include <random>

using namespace std;

class Stock
{
private:
	string name;
	int value;
public:
	Stock();
	Stock(string stockname, int stockvalue);
	~Stock();
	void display();
	void updateValue();
	int getValue();
	string getName();
};

