#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

typedef std::pair<string, int> ownedStock;
class Wallet
{
private:
	vector<ownedStock> portfolio;
	float balance;
public:
	Wallet();
	Wallet(string name);
	Wallet(float x);
	void buyStock(string name, int amount, int value);
	void sellStock(string name, int amount, int value);
	void addStock(string name, int amount);
	void removeStock(int index);
	string getStockName(int index);
	int getStockAmount(int index);
	void displayWallet();
	void saveWallet(string filename);
	~Wallet();
};

