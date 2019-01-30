#pragma once
#include "Stock.h"
#include <iostream>
#include <fstream>
#include <vector>
typedef std::unique_ptr<Stock> UPoint;
class StockList
{
private:
	vector<UPoint> stockList;
public:
	StockList();
	StockList(string filename);
	StockList(int amount);
	void addStock(int newValue);
	void removeStock(int num);
	float displayAll();
	void updateAll();
	string getStockName(int index);
	int getStockValue(int index);
	void saveStocks(string filename);
	~StockList();
};

