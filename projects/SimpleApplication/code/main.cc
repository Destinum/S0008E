#include <iostream>
#include <thread>
#include<cmath>

#include <unistd.h>
#include <sys/utsname.h>

#include <thread>
#include <vector>

using namespace std;


unsigned long int Calculation()
{
	unsigned long int Result = 0;

	for (int i = 1; i <= 50000; i++)
	{
		for (int j = 1; j <= i; j++)
		{
			Result += sqrt(i * j);
		}
	}

	return Result;
}

void ThreadFunction()
{
	cout << "Thread " << this_thread::get_id() << " created. Calculating..." << endl;
	unsigned long int Result = Calculation();
	cout << "Thread " << this_thread::get_id() << " finished calculation. Result: " << Result << endl;
}


int main()
{
	string Command;
	int Value;
	struct utsname SystemInfo;

	cout << "List of Commands: " << endl <<
	"'-i' to show system information." << endl <<
	"'-f' and an integer to run X calculations using forks." << endl <<
	"'-t' and an integer to run X calculations using threads." << endl << endl;

	while(true)
	{
		cout << "Enter Command: ";
		cin >> Command;

		if (!cin.good())
		{
			cout << "Invalid Command." << endl;
		}
		
		else if (Command == "-i")
		{
			char Hostname[1024];
    		gethostname(Hostname, 1024);
			uname(&SystemInfo);

			cout << "Number of Processors: " << std::thread::hardware_concurrency() << endl;
			cout << "Hostname: " << Hostname << endl;
			cout << "Hardware Platform: " << SystemInfo.machine << endl;
			cout << "Total RAM: " << sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGE_SIZE) / pow(10, 9) << " GB" << endl;
		}

		else if (Command == "-f")
		{
			cin >> Value;
			while (!cin.good() || Value < 1)
			{
				cout << "Invalid Value. Enter an integer of 1 or more: ";
				cin.clear();
				cin.ignore(123, '\n');	
				cin >> Value;
			}

			pid_t Parent = getpid();
			pid_t Forks[Value];

			cout << "Parent Process ID: " << Parent << endl;

			for (int i = 0; i < Value; i++)
			{
				Forks[i] = fork();

				if (Parent != getpid())
					break;

				cout << "PID of child #" << i + 1 << ": " << Forks[i] << endl;
			}

			cout << "Process " << getpid() << " finished calculation. Result: " << Calculation() << endl;

			if (Parent != getpid())
			{
				return 0;
			}
			
			sleep(4);
		}

		else if (Command == "-t")
		{
			cin >> Value;
			while (!cin.good() || Value < 1)
			{
				cout << "Invalid Value. Enter an integer of 1 or more: ";
				cin.clear();
				cin.ignore(123, '\n');	
				cin >> Value;
			}
			
			//cout << "Command is " << Command << " with value " << Value << "."<< endl;

			vector<thread> ListOfThreads;

			for (int i = 0; i < Value; i++)
			{
				//thread thread(ThreadFunction);
				//thread.detach();
				//thread(ThreadFunction).detach();

				ListOfThreads.emplace_back(thread(ThreadFunction));
			}

			sleep(1);
			cout << endl;

			for (auto& th : ListOfThreads) 
    			th.join();		
		}

		else
		{
			cout << "Invalid Command." << endl;
		}

		cout << endl;
		cin.clear();
		cin.ignore(123, '\n');	
	}
}