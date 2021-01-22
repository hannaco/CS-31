#include <iostream>
#include <string>
using namespace std;

int main()
{
	cout << "Defendant: "; //Getting the name of the defendant
	string defendant;
	getline(cin, defendant);

	if (defendant == "")
	{
		cout << "---\n";
		cout << "You must enter a defendant name.";
		return 1;
	}

	cout << "Amount paid (in thousands): ";
	double amount;
	cin >> amount;
	cin.ignore(10000, '\n');

	if (amount < 0.00)
	{
		cout << "---\n";
		cout << "The amount paid must not be negative.";
		return 1;
	}

	cout << "Fake athlete? (y/n): ";
	string athleteFaking;

	getline(cin, athleteFaking);

	if (athleteFaking  != "n" && athleteFaking != "y")
	{
		cout << "---\n";
		cout << "You must enter y or n.";
		return 1;
	}

	double fine = 20.0;
	if (amount <= 40.0)
	{
		fine += (0.66) * (amount);
		cout.setf(ios::fixed);
		cout.precision(1);
	}

	if (amount > 40 && amount <= 250)
	{
		fine += (0.66) * (40);
		cout.setf(ios::fixed);
		cout.precision(1);
		double amountModOne = amount - 40.0;
		if (athleteFaking == "y")
		{
			fine += (0.22) * (amountModOne);
		}
		else
			fine += (0.10) * (amountModOne);
	}
	
	if (amount > 250)
	{
		fine += (0.66) * (40);
		cout.setf(ios::fixed);
		cout.precision(1);
		if (athleteFaking == "y")
		{
			fine += (0.22) * (210);
		}
		else
		{
			fine += (0.10) * (210);
		}
		double amountModTwo = amount - 250;
		fine += (0.14) * (amountModTwo);
	}

	cout << "---\n";
	cout << "The suggested fine for " << defendant << " is $" << fine << " thousand." << endl;
}