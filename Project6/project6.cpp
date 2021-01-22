#include <iostream>
using namespace std;

void removeS(char* ptr)
{
	//char* ch = *(ptr);
	//cout << ch << endl;
	//cout << *ptr << endl;
	for (int i = 0; *(ptr + i) != '\0'; i++)
	{
		if (*(ptr + i) == 's' || *(ptr + i) == 'S')
		{
			for (int j = 0; *(ptr + j) != '\0'; j++)
			{
				if (*(ptr + i + j + 1) == '\0')
				{
					*(ptr + i + j) = '\0';
				}
				else
					*(ptr + i + j) = *(ptr + j + i + 1);
			}
			i--;
		}
	}
}

int main()
{
	char msg[50] = "She'll be a massless princess.";
	removeS(msg);
	cout << msg << endl;
}
