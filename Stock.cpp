#include "Stock.h"



Stock::Stock()
{
}

//A stock object contains a name and a value
Stock::Stock(string stockname, int stockvalue)
{
	name = stockname;
	value = stockvalue;
}

Stock::~Stock()
{
}

//display the name and value of a stock in console
void Stock::display()
{
	cout << "stock name: " << this->name << " current value: " << this->value << endl;
}

//updates the value of a stock randomly by -1 or +1
void Stock::updateValue()
{
	if ((rand() % 2) == 0)
	{
		this->value = this->value + 1;
	}
	else {
		this->value = this->value - 1;
	}
}

//returns the value of a stock
int Stock::getValue()
{
	return this->value;
}

//returns the name of a stosk
string Stock::getName()
{
	return this->name;
}

