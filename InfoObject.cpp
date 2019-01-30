#include "InfoObject.h"


//Creates a infoObject with a state and history, appending to the history it's creation
InfoObject::InfoObject()
{
	info = 0;
	history = "";
	history.append("Simulator started \n");
}


InfoObject::~InfoObject()
{
}

//Appends an action to the history of the object by taking in what action was performed, what stock was used and what amount of that stock was handled
void InfoObject::addToHistory(string action, string stockname, int amount)
{
	string temp;
	history.append(action);
	temp = to_string(amount);
	history.append(temp);
	history.append(" ");
	history.append(stockname);
	history.append("\n");
}

//Changes the state of the info object by taking in a integer
void InfoObject::changeInfo(int x)
{
	info = x;
}

//Displays information based on the state of the infoobject
void InfoObject::displayInfo()
{
	switch (info)
	{
	case 0:
		cout << endl << "Press B to buy stocks" << endl << "Press S to sell stocks" << endl << "Press E to exit" <<  endl;
		break;
	case 1:
		cout << endl << "Choose a stock BUY inserting it's index number an pressing ENTER" << endl;
		break;
	case 2:
		cout << endl << "Insert the amount of stocks you want to BUY and press ENTER" << endl;
		break;
	case 3:
		cout << endl << "Choose a stock SELL inserting it's index number an pressing ENTER" << endl;
		break;
	case 4:
		cout << endl << "Insert the amount of stocks you want to SELL and press ENTER" << endl;
		break;
	case 5:
		cout << endl << "Press S to SAVE your game" << endl;
		cout << "Press E to EXIT your game WITHOUT SAVING" << endl;
		cout << endl << "Displaying past actions press anything to continue" << endl << history << endl;
		break;
	}
}
