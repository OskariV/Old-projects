#include "StockList.h"



StockList::StockList()
{
}

//Creates a Stocklist object and fills it's member vector with unique_ptr pointers to stock objects that are given names and values from a file
StockList::StockList(string filename)
{
	ifstream myfile(filename);
	string name;
	string temp;
	int value;
	while (!myfile.eof()) {
		getline(myfile, name);
		getline(myfile, temp);
		value = stoi(temp);
		stockList.push_back(UPoint(new Stock(name, value)));
	}
	myfile.close();
}

//Creates a Stocklist object and adds to it's vector a given amount of stock objects with a value of 100 and a name from a file
StockList::StockList(int amount)
{
	ifstream myfile("stocknames.txt");
	string name;
	for (int i = 0; i < amount; i++)
	{
		getline(myfile, name);
		stockList.push_back(UPoint(new Stock(name, 100)));
	}
	myfile.close();
}

//Adds a stock with a given value and a name from a file
//Compares the new name to existing stock names and chooses another name
void StockList::addStock(int newValue)
{
	ifstream myfile("stocknames.txt");
	string name;
	string compare;
	getline(myfile, name);
	for (int i = 0; i < stockList.size(); i++)
	{
		for (int i = 0; i < stockList.size(); i++)
		{
			compare = stockList[i]->getName();
			if (name == compare)
			{
				getline(myfile, name);
			}
		}
	}
	stockList.push_back(UPoint(new Stock(name, newValue)));
	myfile.close();
}

//removes a given stock
void StockList::removeStock(int num)
{
	stockList.erase(stockList.begin() + num);
}

//uses the display of all stock objects in vector
float StockList::displayAll()
{
	float marketIndex = 0;
	for (int i = 0; i < stockList.size(); i++)
	{
		cout << i << ": ";
		stockList[i]->display();
		marketIndex = marketIndex + stockList[i]->getValue();
	}
	marketIndex = marketIndex / stockList.size() / 100;
	return marketIndex;
}

//uses the update fuction of all stocks and replaces a stock thats value has dropped under 0
void StockList::updateAll()
{
	float compare;
	for (int i = 0; i < stockList.size(); i++)
	{
		stockList[i]->updateValue();
		compare = stockList[i]->getValue();
		if (compare <= 0)
		{
			removeStock(i);
			addStock(100);
		}
	}
}

// gets a stock objects name or returns a failure if trying to acces a object that doesn't exist
string StockList::getStockName(int index)
{
	if (index < stockList.size())
	{
		return stockList[index]->getName();
	}
	else {
		return "failure";
	}
}

// gets a stock objects value or returns a failure if trying to access a object that doesn't exist
int StockList::getStockValue(int index)
{
	if (index < stockList.size())
	{
		return stockList[index]->getValue();
	}
	else {
		return 0;
	}
}

//Saves all stock objects names and value to a file
//this funtion rewrites the data already existing in the file
void StockList::saveStocks(string filename)
{
	ofstream savefile(filename, ios::out | ios::trunc);
	for (int i = 0; i < stockList.size(); i++)
	{
		savefile << stockList[i]->getName() << endl;
		savefile << stockList[i]->getValue() << endl;
	}
	savefile.close();
}


StockList::~StockList()
{
}
