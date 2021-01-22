#include "grid.h"
#include <iostream>
#include <string>
#include <cctype>
#include <sstream>

using namespace std;

const int HORIZ = 0;
const int VERT = 1;

const int FG = 0;
const int BG = 1;

void plotVerticalLine(int r, int c, int distance, char ch, int ground);
void plotHorizontalLine(int r, int c, int distance, char ch, int ground);
bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg);
int performCommands(string commandString, char& plotChar, int& mode, int& badPos);
void performC(int& pos, char& cur, int& fgbg, int& beginRow, int& beginCol);
bool processFB(string text, int& pos, char& cur, int& fgbg, int& yikes);
bool processHV(string text, int& pos, char& cur, int& fgbg, int& yikes, int& beginRow, int& beginCol, int& change);
bool performFB(string text, int& pos, char& cur, int& fgbg, int& yikes);
bool performHV(string text, int& pos, char& cur, int& fgbg, int& yikes, int& beginRow, int& beginCol, int& change);
bool inBounds(char hv, int y, int x, int traverse);
int checkSyntax(string text, char cur, int fgbg, int& yikes);

int main()
{
	setSize(20, 30);
	clearGrid();
	char currentChar = '*';
	int currentMode = FG;
	for (;;)
	{
		cout << "Enter a command string: ";
		string cmd;
		getline(cin, cmd);
		if (cmd == "")
			break;
		int position;
		int status = performCommands(cmd, currentChar, currentMode, position);
		switch (status)
		{
		case 0:
			draw();
			break;
		case 1:
			cout << "Syntax error at position " << position + 1 << endl;
			break;
		case 2:
			cout << "Cannot perform command at position " << position + 1 << endl;
			break;
		default:
			// It should be impossible to get here.
			cerr << "performCommands returned " << status << "!" << endl;
		}
	}
}

void plotHorizontalLine(int r, int c, int distance, char ch, int ground)
{
	if (ground == FG)
	{
		int last = c + distance;
		if (distance >= 0)
		{
			for (int i = c; i <= last; i++)
			{
				setChar(r, i, ch);
			}
		}
		else
		{
			for (int i = c; i >= last; i--)
			{
				setChar(r, i, ch);
			}
		}
	}
	else
	{
		int last = c + distance;
		if (distance >= 0)
		{
			for (int i = c; i <= last; i++)
			{
				if (getChar(r, i) == ' ')
				{
					setChar(r, i, ch);
				}
			}
		}
		else
		{
			for (int i = c; i >= last; i--)
			{
				if (getChar(r, i) == ' ')
				{
					setChar(r, i, ch);
				}
			}
		}
	}
}

void plotVerticalLine(int r, int c, int distance, char ch, int ground)
{
	if (ground == FG)
	{
		int last = r + distance;
		if (distance >= 0)
		{
			for (int i = r; i <= last; i++)
			{
				setChar(i, c, ch);
			}
		}
		else
		{
			for (int i = r; i >= last; i--)
			{
				setChar(i, c, ch);
			}
		}
	}
	else
	{
		int last = r + distance;
		if (distance >= 0)
		{
			for (int i = r; i <= last; i++)
			{
				if (getChar(i, c) == ' ')
				{
					setChar(i, c, ch);
				}
			}
		}
		else
		{
			for (int i = r; i >= last; i--)
			{
				if (getChar(i, c) == ' ')
				{
					setChar(i, c, ch);
				}
			}
		}
	}
}

bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg)
{
	if (dir != HORIZ && dir != VERT)
		return false;

	if (fgbg != FG && fgbg != BG)
		return false;

	if (dir == HORIZ)
	{
		if ((c + distance) > 30 || (c + distance) < 0)
			return false;
	}

	if (dir == VERT)
	{
		if ((r + dir) > 30 || (r + dir) < 0)
			return false;
	}

	if (!isprint(plotChar))
		return false;

	if (dir == HORIZ)
	{
		plotHorizontalLine(r, c, distance, plotChar, fgbg);
	}
	else
	{
		plotVerticalLine(r, c, distance, plotChar, fgbg);
	}
	return true;
}

void performC(int& pos, char& cur, int& fgbg, int& beginRow, int& beginCol)
{
	clearGrid();
	cur = '*';
	fgbg = FG;
	beginRow = 1;
	beginCol = 1;
	pos++;
}

bool processFB(string text, int& pos, char& cur, int& fgbg, int& yikes)
{
	int end = text.size() - 1;
	if (pos == end)		//if its the last character in the string, then it's a syntax error
	{
		yikes = pos;
		return false;
	}

	if (!isprint(text[pos + 1]))	//if the following character(plotChar) is not printable, then syntax error
	{
		yikes = pos + 1;
		return false;
	}
	pos += 2;		//increments pos(k) by two, moving on to the next command
	return true;
}

bool performFB(string text, int& pos, char& cur, int& fgbg, int& yikes)
{
	cur = text[pos + 1];	//changes plotChar
	if (text[pos] == 'f' || text[pos] == 'F')
		fgbg = FG;		//changes mode to FG
	else
		fgbg = BG;		//changes mode to BG
	return true;
}

bool inBounds(char hv, int y, int x, int traverse)
{
	if (hv == 'h' || hv == 'H')
	{
		if (x + traverse > 30 || x + traverse < 1)		//if it attempts to plot the line outside the grid, plotting error
			return false;
	}

	if (hv == 'v' || hv == 'V')
	{
		if (y + traverse > 20 || y + traverse < 1)		//if it attempts to plot the line outside the grid, plotting error
			return false;
	}

	return true;
}

bool processHV(string text, int& pos, char& cur, int& fgbg, int& yikes, int& beginRow, int& beginCol, int& change)
{
	if (pos == text.size() - 1)		//if it is the last character of the commandString
	{
		yikes = pos;				//changes value of badPos and returns a syntax error
		return false;
	}
	string distance = "";
	if (text[pos + 1] == '-')		//if the length is negative
	{
		distance += text[pos + 1];		
		int i = pos + 2;
		while (isdigit(text[i]))	//while the characters following '-' are digits
		{
			distance += text[i];		//add those characters onto the string digit
			i++;
		}
		stringstream ss(distance);
		if (distance.size() > 3)		//if the plotting command is longer than two digits, syntax error
		{
			yikes = pos + 4;
			return false;
		}
	}
	else
	{
		int i = pos + 1;
		while (isdigit(text[i]))		//while the characters following 'h' or 'v' are digits
		{
			distance += text[i];		//add them onto distance
			i++;
		}
		stringstream ss(distance);
		if (distance.size() > 2)		//if the length is more than two digits, return a syntax error
		{
			yikes = pos + 3;
			return false;
		}
	}
	pos += distance.size() + 1;		//increment pos (k) to the next command
	return true;
}

bool performHV(string text, int& pos, char& cur, int& fgbg, int& yikes, int& beginRow, int& beginCol, int& change)
{
	string distance = "";
	if (text[pos + 1] == '-')		//this is for if the distance is negative
	{
		distance += text[pos + 1];
		int i = pos + 2;
		while (isdigit(text[i]))		//adds the characters that are digits onto distance
		{
			distance += text[i];
			i++;
		}
		stringstream ss(distance);
		int length = 0;
		ss >> length;
		if (!(inBounds(text[pos], beginRow, beginCol, length)))		// if it plots out of bounds, plotting error
		{
			yikes = pos;
			return false;
		}
		if (text[pos] == 'h' || text[pos] == 'H')
		{
			plotHorizontalLine(beginRow, beginCol, length, cur, fgbg);
			beginCol += length;
		}
		else
		{
			plotVerticalLine(beginRow, beginCol, length, cur, fgbg);
			beginRow += length;
		}
	}
	else					//this is for if the direction is positive
	{
		int i = pos + 1;
		while (isdigit(text[i]))
		{
			distance += text[i];
			i++;
		}
		stringstream ss(distance);
		int length = 0;
		ss >> length;
		if (!inBounds(text[pos], beginRow, beginCol, length))
		{
			yikes = pos;
			return false;
		}

		if (text[pos] == 'h' || text[pos] == 'H')
		{
			plotHorizontalLine(beginRow, beginCol, length, cur, fgbg);
			beginCol += length;
		}
		else
		{
			plotVerticalLine(beginRow, beginCol, length, cur, fgbg);
			beginRow += length;
		}
	}
	change = distance.size() + 1;
	return true;
}

int checkSyntax(string text, char cur, int fgbg, int& yikes)
{
	int startRow = 1;		//row where the line will be plotted from
	int startCol = 1;		//column where line will be plotted from

	int k = 0;
	while (k != text.size())	//traverses the commandString
	{
		if (text[k] == 'c' || text[k] == 'C')
		{
			k++;
		}

		else if (text[k] == 'f' || text[k] == 'F' || text[k] == 'b' || text[k] == 'B')
		{
			if (!processFB(text, k, cur, fgbg, yikes))		//checks the syntax if the command begins with 'f' or 'b'
				return 1;
		}

		else if (text[k] == 'h' || text[k] == 'H' || text[k] == 'v' || text[k] == 'V')
		{
			int total;
			if (!(isdigit(text[k + 1]) || text[k + 1] == '-'))		//if the character after 'h' or 'v' is not '-' or digit, syntax error
			{
				yikes = k + 1;
				return 1;
			}
			if (!processHV(text, k, cur, fgbg, yikes, startRow, startCol, total))		//checks syntax of command that begins with 'h' or 'v'
				return 1;
		}
		else if (text[k] != 'H' || text[k] != 'h' || text[k] != 'V' || text[k] != 'v' || text[k] != 'F' || text[k] != 'f' || text[k] != 'B' || text[k] != 'b' || text[k] != 'C' || text[k] != 'c')		//if the character is not on of the possible beginnings of a command, synax error
		{
			yikes = k;
			return 1;
		}
		else
			return 0;
	}
	return 0;
}

int performCommands(string commandString, char& plotChar, int& mode, int& badPos)
{
	if (commandString == "")		//returns if the commandString is empty
		return 0;

	int startRow = 1;
	int startCol = 1;

	if (checkSyntax(commandString, plotChar, mode, badPos) == 1)	//checking syntax of the string
		return 1;

	int k = 0;
	while (k != commandString.size())			//traverses commandString
	{
		if (commandString[k] == 'c' || commandString[k] == 'C')
		{
			performC(k, plotChar, mode, startRow, startCol);		//clears the grid
		}

		else if (commandString[k] == 'f' || commandString[k] == 'F' || commandString[k] == 'b' || commandString[k] == 'B')
		{
			performFB(commandString, k, plotChar, mode, badPos);		//changes mode and plotChar
			k += 2;		//moves on to the next command
		}

		else if (commandString[k] == 'h' || commandString[k] == 'H' || commandString[k] == 'v' || commandString[k] == 'V')
		{
			int total;		//variable meant to represent how long the command is
			if (!performHV(commandString, k, plotChar, mode, badPos, startRow, startCol, total))		//tries to plot line, returns a plotting error if out of bounds
				return 2;
			k += total;		//moves onto the next command
		}
	}
	return 0;
}


