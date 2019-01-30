#pragma once
class MarketActive
{
	bool isActive;
public:
	void changeState();
	bool getState();
	MarketActive();
	~MarketActive();
};

