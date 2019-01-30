//Oskari Virtanen ID: 1706816
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include <iostream>
#include <random>
#include <vector>
#include <time.h>
#include <algorithm>
#include <thread>
#include <conio.h>
#include "Stock.h"
#include "Market.h"

using namespace std;

//Function for the thread object that updates stockValues and displays information
//Refreshes every second based on my systems clock where my clock_t value of 1000 is aproximately one second
//Thread remains active until the market objects marketActive state changes
void threadTest(Market m)
{
	clock_t t = clock();
	while (m.getState() == true)
	{	
		if (clock() > t + 1000)
		{
			m.updateAll();
			t = clock();
			system("cls");
			m.displayAll();
		}
	}
}

//Main game function for new game that sets the stock values at 100 and give the player a balance of 10000
//Function takes players input and based on that input displays relevant information on what the player can do
//At the beginning the funtion creates a market object and a thread object so that the display and update functions can be used independently
//When the player decides to quit the game they can choose to either save their data or discard it
//When the function is quit the thread is deactivated with changeState() and thread.join() funtions so that the data can be saved and game quit safely
void newGame()
{
	Market market(10, 10000);
	bool game = true;
	bool ending = true;
	int choice,whichStock,howMany;
	thread t1(threadTest, market);
	while (game)
	{
		choice = _getch();
		market.changeInfo(0);
		switch (choice)
		{
		case 'b':
			market.changeInfo(1);
			cin >> whichStock;
			market.changeInfo(2);
			cin >> howMany;
			market.buy(whichStock, howMany);
			break;
		case 's':
			market.changeInfo(3);
			cin >> whichStock;
			market.changeInfo(4);
			cin >> howMany;
			market.sell(whichStock, howMany);
			break;
		case 'e':
			market.changeInfo(5);
			market.changeState();
			t1.join();
			system("cls");
			market.displayAll();
			while (ending)
			{
				choice = _getch();
				switch (choice)
				{
				case 's':
					market.save();
					ending = false;
					game = false;
					break;
				case 'e':
					ending = false;
					game = false;
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}
	}
}

//Loadgame functions is the same as the newgame except that it creates market from savefile data
void loadGame()
{
	Market market("savestocklist.txt", "savewallet.txt");
	bool game = true;
	bool ending = true;
	int choice, whichStock, howMany;
	thread t1(threadTest, market);
	while (game)
	{
		choice = _getch();
		market.changeInfo(0);
		switch (choice)
		{
		case 'b':
			market.changeInfo(1);
			cin >> whichStock;
			market.changeInfo(2);
			cin >> howMany;
			market.buy(whichStock, howMany);
			break;
		case 's':
			market.changeInfo(3);
			cin >> whichStock;
			market.changeInfo(4);
			cin >> howMany;
			market.sell(whichStock, howMany);
			break;
		case 'e':
			market.changeInfo(5);
			market.changeState();
			t1.join();
			system("cls");
			market.displayAll();
			while (ending)
			{
				choice = _getch();
				switch (choice)
				{
				case 's':
					market.save();
					ending = false;
					game = false;
					break;
				case 'e':
					ending = false;
					game = false;
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}
	}
}

//Game start sets up the rand() for stocks so their values can update
//From gamestart player can choose to start a new game, load one from savefiles or exit the game
//After the active game is exited the player returns here
void gameStart()
{
	srand(time(NULL));
	bool game = true;
	string choice;
	while (game)
	{
		system("cls");
		cout << "Stock market simulator" << endl << "oskari.v" << endl << endl;
		cout << "Enter [new] to start a new game" << endl << "Enter [load] to resume a saved session" << endl;
		cout << "Enter [exit] to quit" << endl;
		cin >> choice;
		if (choice == "new")
		{
			newGame();
		}
		else if (choice == "load")
		{
			loadGame();
		}
		else if (choice == "exit")
		{
			game = false;
		}
		else {
			cout << "Enter valid command" << endl;
			system("pause");
			system("cls");
		}
	}
}

//checks for memory leaks and starts the program
void main()
{
	gameStart();
#ifdef _DEBUG
	if (_CrtDumpMemoryLeaks())
	{
		cout << endl << "leaks" << endl;
	}
	else {
		cout << endl << "toimii" << endl;
	}
#endif
	system("pause");
}