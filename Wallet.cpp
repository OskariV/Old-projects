#include "Wallet.h"



Wallet::Wallet()
{
}

//Creates a wallet object and fills it with data from a given file
//Saved data includes owned stocks and the wallets balance
Wallet::Wallet(string filename)
{
	ifstream myfile(filename);
	string name;
	string temp;
	int amount;
	getline(myfile, temp);
	balance = stoi(temp);
	while (!myfile.eof()) {
		getline(myfile, name);
		getline(myfile, temp);
		amount = stoi(temp);
		portfolio.push_back(ownedStock(name, amount));
	}
	myfile.close();
}

//Creates a empty wallet with a given balance
Wallet::Wallet(float x)
{
	balance = x;
}

//Uses given name and value to add a stock to portfolio vector and calculating how much money is lost from the balance
//If balance isn't high enough to buy stocks it sends an error
void Wallet::buyStock(string name, int amount, int value)
{
	if (balance >= (balance - value * amount))
	{
		balance = (balance - value * amount);
		portfolio.push_back(ownedStock(name, amount));
	} else {
		cout << "not enough money" << endl;
	}
}

//Uses given name and value to remove stocks from portfolio vector and calculating how much money is added to the balance
//If the stock is not owned or user tries to sell more then they own function sends an error
void Wallet::sellStock(string name, int amount, int value)
{
	for (int index = 0; index < portfolio.size(); index++)
	{
		if (name == portfolio[index].first)
		{
			if (amount <= portfolio[index].second)
			{
				balance = balance + value * amount;
				portfolio[index].second = portfolio[index].second - amount;
				if (portfolio[index].second <= 0)
				{
					portfolio.erase(portfolio.begin() + index);
				}
			}
			else {
				cout << "not enough stocks" << endl;
			}
		}
		else {
			cout << "Stock not in portfolio" << endl;
		}
	}
}

//Adds stock with a name and amount owned to the prtfolio vector
void Wallet::addStock(string name, int amount)
{
	portfolio.push_back(ownedStock(name, amount));
}

//removes a stock from portfolio
void Wallet::removeStock(int index)
{
	portfolio.erase(portfolio.begin() + index);
}

//gets a stocks name from the portfolio
string Wallet::getStockName(int index)
{
	return portfolio[index].first;
}

//gets the amount owned of a given stock
int Wallet::getStockAmount(int index)
{
	return portfolio[index].second;
}

//Prints the name and amount owned of all stocks in portfolio
void Wallet::displayWallet()
{
	cout << endl << "Your portfolio:" << endl;
	for (int i = 0; i < portfolio.size(); i++)
	{
		cout << portfolio[i].first << " amount owned: " << portfolio[i].second << endl;
	}
	cout << "Your balance: " << balance << endl;
}

//Saves the wallets balance and information for all stocks in portfolio in to a given file
void Wallet::saveWallet(string filename)
{
	ofstream savefile(filename, ios::out | ios::trunc);
	savefile << balance << endl;
	for (int i = 0; i < portfolio.size(); i++)
	{
		savefile << portfolio[i].first << endl;
		savefile << portfolio[i].second << endl;
	}
	savefile.close();
}


Wallet::~Wallet()
{
}

