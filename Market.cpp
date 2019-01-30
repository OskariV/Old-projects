#include "Market.h"



Market::Market()
{
}

//Creates market and other objects by taking the new balance and amount of stocks to create
Market::Market(int stocks, int balance):state(new MarketActive()),stockList(new StockList(stocks)),wallet(new Wallet(balance)),info(new InfoObject())
{
	marketIndex = 0;
}

//Creates market and other objects from data saved in files
Market::Market(string fileforstocks, string fileforwallet) :state(new MarketActive()), stockList(new StockList(fileforstocks)), wallet(new Wallet(fileforwallet)), info(new InfoObject())
{
	marketIndex = 0;
}

//Adds stocks used in testing
void Market::addStockToMarket(int newValue)
{
	stockList->addStock(newValue);
}

//Removes stocks used in testing
void Market::removeStockFromMarket(int num)
{
	stockList->removeStock(num);
}

//Displays all information from wallet and stocklist by using their display functions
void Market::displayAll()
{
	marketIndex = stockList->displayAll();
	cout << "index: " << marketIndex << endl;
	if (getState())
	{
		cout << "active" << endl;
	}
	else {
		cout << "inactive" << endl;
	}
	wallet->displayWallet();
	info->displayInfo();
}

//Updates all stocks
void Market::updateAll()
{
	stockList->updateAll();
}

//Changes the state of the object that is used to control threads
void Market::changeState()
{
	state->changeState();
}

//Uses the infoobjects function to change what info is displayed
void Market::changeInfo(int x)
{
	info->changeInfo(x);
}

//Gets the state of the object that is used to control thread
bool Market::getState()
{
	return state->getState();
}

//Uses the buy function of wallet by using given amount and getting the name and value of a stock from stocklist
//Function uses the index value to access a specific stock and stops the process if the stock user tries to access doesn't exist
//Uses infoobjects addToHistory fuction to save the action to memory
void Market::buy(int index, int amount)
{
	string name = stockList->getStockName(index);
	if (name != "failure")
	{
		wallet->buyStock(name, amount, stockList->getStockValue(index));
		info->addToHistory("bought ", name, stockList->getStockValue(index));
	}
}

//Uses the sell function of wallet by using given amount and getting the name and value of a stock from stocklist
//Function uses the index value to access a specific stock and stops the process if the stock user tries to access doesn't exist
//Uses infoobjects addToHistory fuction to save the action to memory
void Market::sell(int index, int amount)
{
	string name = stockList->getStockName(index);
	if (name != "failure")
	{
		wallet->sellStock(name, amount, stockList->getStockValue(index));
		info->addToHistory("sold ", name, stockList->getStockValue(index));
	}
}

//Uses the save functions of stocklist and wallet
void Market::save()
{
	stockList->saveStocks("savestocklist.txt");
	wallet->saveWallet("savewallet.txt");
}


Market::~Market()
{
}
