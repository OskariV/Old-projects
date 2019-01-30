#pragma once
#include "StockList.h"
#include "MarketActive.h"
#include "Wallet.h"
#include "InfoObject.h"
#include <iostream>
#include <vector>
class Market
{
private:
	//Shared_ptr is used instead of unique_ptr because thread objects constructor uses copies instead of the actual object 
	//so to access values in both thread and main program shared_ptr has to be used
	shared_ptr<StockList> stockList;
	shared_ptr<Wallet> wallet;
	shared_ptr<MarketActive> state;
	shared_ptr<InfoObject> info;
	float marketIndex;
public:
	Market();
	Market(int stocks, int balance);
	Market(string fileforstocks, string fileforwallet);
	void addStockToMarket(int newValue);
	void removeStockFromMarket(int num);
	void displayAll();
	void updateAll();
	void changeState();
	void changeInfo(int x);
	bool getState();
	void buy(int index, int amount);
	void sell(int index, int amount);
	void save();
	~Market();
};

