#include <iostream>
using namespace std;

/*
int main()
{
	std::string Command;
	while(true)
	{
		cout << "Enter Command: ";
		cin >> Command;
		if (Command == "Fisk" || Command == "1" || Command == "Mås")
		{
			cout << "Command is " << Command << "."<< endl;
		}
		else
		{
			cout << "Unrecognized Command." << endl;
		}
		cout << endl;	
	}
}
*/

int main()
{
	//int Command;
	string Command;
	while(true)
	{
		cout << "Enter Command: ";
		cin >> Command;

		if (!cin.good())
		{
			cin.clear();
			cin.ignore(123, '\n');
			cout << "Invalid Command." << endl;
		}
		
		else if (Command == "-i")
		{
			cout << "Command is " << Command << "."<< endl;
		}

		else if (Command == "-f")
		{
			cout << "Command is " << Command << "."<< endl;
		}

		else if (Command == "-t")
		{
			cout << "Command is " << Command << "."<< endl;
		}

		else
		{
			cout << "Invalid Command." << endl;
		}

		cout << endl;	
	}
}