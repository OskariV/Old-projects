#pragma once
#include <iostream>
#include <string>

using namespace std;
class InfoObject
{
private:
	int info;
	string history;
public:
	InfoObject();
	~InfoObject();
	void addToHistory(string action, string stockname, int amount);
	void changeInfo(int x);
	void displayInfo();
};

