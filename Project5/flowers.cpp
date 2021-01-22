#define _CRT_SECURE_NO_DEPRECATE
#include "utilities.h"
#include <iostream>
#include <cstring>

using namespace std;

const char WORDFILENAME[] = "C:/Users/Hanna/Desktop/UCLA/CS31/Project 5/words.txt";
const int MAXWORDS = 10000;

int playOneRound(const char words[][MAXWORDLEN + 1], int nWords, int wordnum);

int main()
{
	char w[MAXWORDS][MAXWORDLEN + 1];									//initialize w
	int n = getWords(w, MAXWORDS, WORDFILENAME);						//load words into w
	bool canPlay = true;												//boolean to determine if words loaded properly and if the number of rounds is positive
	int rounds;	

	if (n > MAXWORDS || n < 1)											//cannot play game if words weren't loaded properly
	{
		cout << "No words were loaded, so I can't play the game." << endl;
		canPlay = false;
	}

	if (canPlay)														//determine how many rounds the user wants to play and store into rounds
	{
		cout << "How many rounds do you want to play? ";
		cin >> rounds;
		cin.ignore(10000, '\n');

		cout << '\n';
	}

	if (rounds < 0)														//cannot play game if number of rounds is negative
	{
		cout << "The number of rounds must be positive." << endl;
		canPlay = false;
	}

	if (canPlay)
	{
		int total = 0;
		double average;
		int min	;
		int max = 0;
		int cur;

		int played = 1;
		while (played <= rounds)										//executes the following code rounds number of times
		{
			int num = randInt(0, n - 1);								//choosing a word
			int length = 0;
			for (int i = 0; w[num][i] != '\0'; i++)
			{
				length++;												//finding how many letters the myster word is
			}
			cout << "Round " << played << endl;
			cout << "The mystery word is " << length << " letters long." << endl;
			cout << w[num] << endl;
			cur = playOneRound(w, n, num);								
			if (cur == -1)												//if the index of mystery word is not valid, cannot play game
			{
				cout << "Unable to play the game." << endl;
				break;
			}
			total += cur;												//total number of tries in all the rounds
			if (played == 1)
				min = cur;
			else
			{
				if (cur < min)												//changes the value of minimum if there is a new min
					min = cur;
			}
			if (cur > max)												//changes value of max if there is a new max
				max = cur;
			average = static_cast<double>(total) / played;				//calculates average number of tries for all the rounds that have been played
			cout.setf(ios::fixed);
			cout.precision(2);
			if (cur == 1)
				cout << "You got it in 1 try." << endl;
			else
				cout << "You got it in " << cur << " tries" << endl;
			cout << "Average: " << average << ", minimum: " << min << ", maximum: " << max << endl;
			played++;
			cout << '\n';
		}
	}
}

int playOneRound(const char words[][MAXWORDLEN + 1], int nWords, int wordnum)
{
	int tries = 0;
	bool correct = false;
	if (wordnum < 0 || wordnum >= nWords)							//error message if wordnum is invalid index
		return -1;
	while (!correct)												//continues to ask for trial words as long as it is not the mystery word
	{
		char guess[10000] = {};	
		int length = 0;
		char copy[MAXWORDLEN + 1] = {};
		for (int i = 0; words[wordnum][i] != '\0'; i++)				//gets length of mystery word
		{
			length++;
		}
		for (int i = 0; i <= length; i++)							//copies mystery word into another char array
			copy[i] = words[wordnum][i];			
		cout << "Trial word: ";
		cin.getline(guess, 10000);
		bool exists = false;
		bool wordOk = true;
		bool validTrial = true;
		int letters = 0;	
		int flowers = 0;
		int bees = 0;

		for (int i = 0; guess[i] != '\0'; i++)						//tests to see if the trial words only consists of 4-6 lowercase letters
		{															//if it doesn't, prints arror message, doesn't count it as a try
			if (!isalpha(guess[i]))
			{
				wordOk = false;
			}
			else
			{
				if (!islower(guess[i]))
				{
					wordOk = false;
					break;
				}
				letters++;
			}
		}

		if (letters < 4 || letters > 6)
			wordOk = false;

		if (!wordOk)
		{
			cout << "Your trial word must be a word of 4 to 6 lower case letters." << endl;
			validTrial = false;
		}

		if (wordOk)												//if it is 4-6 lowercase letters, tests to see if it is in the array of valif words
		{														//if it is not, prints arror message, doesn't count it as a try
			for (int i = 0; i < nWords; i++)
			{
				if (strcmp(words[i], guess) == 0)
				{
					exists = true;
					break;
				}
			}

			if (!exists)
			{
				cout << "I don't know that word." << endl;
				validTrial = false;
			}
		}

		if (validTrial)											//if it is a valid trial word, then evaluates for correctedness
		{
			if (strcmp(words[wordnum], guess) == 0)				//if trial word is mystery word, increments number of tries and break out of the loop
			{
				correct = true;
				tries++;
				break;
			}

			for (int i = 0; guess[i] != '\0'; i++)				//looking for flowers
			{
				if (words[wordnum][i] == guess[i])				//if it is a flower, changes the character in position i in both cstrings to '*'
				{
					copy[i] = '*';
					guess[i] = '*';
					flowers++;									//increments number of flowers
				}
			}

			for (int i = 0; guess[i] != '\0'; i++)				//looking for bees
			{
				for (int j = 0; words[wordnum][j] != '\0'; j++)	//if it is a bee, changes the char at i in trial to '*', and char at j in copy to '*'
				{
					if (guess[i] == copy[j])
					{
						if (guess[i] != '*' && copy[j] != '*')	//if it matches, and is not '*', then increments number of bees
						{
							bees++;
							guess[i] = '*';
							copy[j] = '*';
						}
					}
				}
			}

			cout << "Flowers: " << flowers << ", Bees: " << bees << endl;
			tries++;
		}
	}
	return tries;
}