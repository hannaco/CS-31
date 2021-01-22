// vampires.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the utterly trivial
// functions (marked TRIVIAL).  Then get Arena::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;                // max number of rows in the arena
const int MAXCOLS = 20;                // max number of columns in the arena
const int MAXVAMPIRES = 100;           // max number of vampires allowed

const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;
const int NUMDIRS = 4;

const int EMPTY = 0;
const int HAS_POISON = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
			  // type name, since it's mentioned in the Vampire declaration.

class Vampire
{
public:
	// Constructor
	Vampire(Arena* ap, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	bool isDead() const;

	// Mutators
	void move();

private:
	Arena* m_arena;
	int    m_row;
	int    m_col;
	int timesPoisoned;
	bool canMove;
	bool dead;

	// TODO: You'll probably find that a vampire object needs additional
	// data members to support your implementation of the behavior affected
	// by poisoned blood vials.
};

class Player
{
public:
	// Constructor
	Player(Arena* ap, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	bool isDead() const;

	// Mutators
	string dropPoisonVial();
	string move(int dir);
	void   setDead();

private:
	Arena* m_arena;
	int    m_row;
	int    m_col;
	bool   m_dead;
};

class Arena
{
public:
	// Constructor/destructor
	Arena(int nRows, int nCols);
	~Arena();

	// Accessors
	int     rows() const;
	int     cols() const;
	Player* player() const;
	int     vampireCount() const;
	int     getCellStatus(int r, int c) const;
	int     numberOfVampiresAt(int r, int c) const;
	void    display(string msg) const;

	// Mutators
	void setCellStatus(int r, int c, int status);
	bool addVampire(int r, int c);
	bool addPlayer(int r, int c);
	void moveVampires();

private:
	int      m_grid[MAXROWS][MAXCOLS];
	int      m_rows;
	int      m_cols;
	Player* m_player;
	Vampire* m_vampires[MAXVAMPIRES];
	int      m_nVampires;
	int      m_turns;

	// Helper functions
	void checkPos(int r, int c, string functionName) const;
	bool isPosInBounds(int r, int c) const;
};

class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nVampires);
	~Game();

	// Mutators
	void play();

private:
	Arena* m_arena;

	// Helper functions
	string takePlayerTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int lowest, int highest);
bool decodeDirection(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Vampire implementation
///////////////////////////////////////////////////////////////////////////

Vampire::Vampire(Arena* ap, int r, int c)
{
	if (ap == nullptr)
	{
		cout << "***** A vampire must be created in some Arena!" << endl;
		exit(1);
	}
	if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
	{
		cout << "***** Vampire created with invalid coordinates (" << r << ","
			<< c << ")!" << endl;
		exit(1);
	}
	m_arena = ap;
	m_row = r;
	m_col = c;
	timesPoisoned = 0;
	canMove = true;
	dead = false;
}

int Vampire::row() const
{
	return m_row;
}

int Vampire::col() const
{
	return m_col;
}

bool Vampire::isDead() const
{
	return dead;
}

void Vampire::move()
{
	if (!canMove)
	{
		canMove = true;
	}
	else
	{
		int moveDir = randInt(0, 3);
		attemptMove(*m_arena, moveDir, m_row, m_col);
		if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
		{
			timesPoisoned++;
			m_arena->setCellStatus(m_row, m_col, EMPTY);
		}
		if (timesPoisoned == 2)
			dead = true;
		if (timesPoisoned == 1 && canMove)
			canMove = false;
	}
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
	if (ap == nullptr)
	{
		cout << "***** The player must be created in some Arena!" << endl;
		exit(1);
	}
	if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
	{
		cout << "**** Player created with invalid coordinates (" << r
			<< "," << c << ")!" << endl;
		exit(1);
	}
	m_arena = ap;
	m_row = r;
	m_col = c;
	m_dead = false;
}

int Player::row() const
{
	return m_row;
}

int Player::col() const
{
	return m_col;
}

string Player::dropPoisonVial()
{
	if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
		return "There's already a poisoned blood vial at this spot.";
	m_arena->setCellStatus(m_row, m_col, HAS_POISON);
	return "A poisoned blood vial has been dropped.";
}

string Player::move(int dir)
{
	int status = m_arena->getCellStatus(m_row, m_col);
	if (!attemptMove(*m_arena, dir, m_row, m_col))
		return "Player couldn't move; player stands.";
	else if (status == 'V' || (status >= 2 && status <= 9))
	{
		setDead();
		return "Player walked into a vampire and died.";
	}
	else if (dir == NORTH)
		return "Player moved north.";
	else if (dir == EAST)
		return "Player moved east.";
	else if (dir == SOUTH)
		return "Player moved south.";
	else
		return "Player moved west.";
}

bool Player::isDead() const
{
	return m_dead;
}

void Player::setDead()
{
	m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
	if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
	{
		cout << "***** Arena created with invalid size " << nRows << " by "
			<< nCols << "!" << endl;
		exit(1);
	}
	m_rows = nRows;
	m_cols = nCols;
	m_player = nullptr;
	m_nVampires = 0;
	m_turns = 0;
	for (int r = 1; r <= m_rows; r++)
		for (int c = 1; c <= m_cols; c++)
			setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
	delete m_player;
	for (int i = 0; i < m_nVampires; i++)
		delete m_vampires[i];
}

int Arena::rows() const
{
	return m_rows;
}

int Arena::cols() const
{
	return m_cols;
}

Player* Arena::player() const
{
	return m_player;
}

int Arena::vampireCount() const
{
	return m_nVampires;
}

int Arena::getCellStatus(int r, int c) const
{
	checkPos(r, c, "Arena::getCellStatus");
	return m_grid[r - 1][c - 1];
}

int Arena::numberOfVampiresAt(int r, int c) const
{
	int count = 0;
	for (int i = 0; i < m_nVampires; i++)
	{
		if (m_vampires[i]->col() == c && m_vampires[i]->row() == r)
			count++;
	}
	return count;
}

void Arena::display(string msg) const
{
	char displayGrid[MAXROWS][MAXCOLS];
	int r, c;

	// Fill displayGrid with dots (empty) and stars (poisoned blood vials)
	for (r = 1; r <= rows(); r++)
		for (c = 1; c <= cols(); c++)
			displayGrid[r - 1][c - 1] = (getCellStatus(r, c) == EMPTY ? '.' : '*');

	for (int i = 0; i < m_nVampires; i++)
	{
		int vRow = m_vampires[i]->row();
		int vCol = m_vampires[i]->col();
		int count = numberOfVampiresAt(vRow, vCol);
		if (count > 9)
			count = 9;
		char c = '0' + count;
		if (count == 1)
			displayGrid[vRow - 1][vCol - 1] = 'V';
		else
			displayGrid[vRow - 1][vCol - 1] = c;
	}

	// Indicate player's position
	if (m_player != nullptr)
		displayGrid[m_player->row() - 1][m_player->col() - 1] = (m_player->isDead() ? 'X' : '@');

	// Draw the grid
	clearScreen();
	for (r = 1; r <= rows(); r++)
	{
		for (c = 1; c <= cols(); c++)
			cout << displayGrid[r - 1][c - 1];
		cout << endl;
	}
	cout << endl;

	// Write message, vampire, and player info
	if (msg != "")
		cout << msg << endl;
	cout << "There are " << vampireCount() << " vampires remaining." << endl;
	if (m_player == nullptr)
		cout << "There is no player!" << endl;
	else if (m_player->isDead())
		cout << "The player is dead." << endl;
	cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
	checkPos(r, c, "Arena::setCellStatus");
	m_grid[r - 1][c - 1] = status;
}

bool Arena::addVampire(int r, int c)
{
	if (!isPosInBounds(r, c))
		return false;

	// Don't add a vampire on a spot with a poisoned blood vial
	if (getCellStatus(r, c) != EMPTY)
		return false;

	// Don't add a vampire on a spot with a player
	if (m_player != nullptr && m_player->row() == r && m_player->col() == c)
		return false;

	if (m_nVampires == MAXVAMPIRES)
		return false;
	else
	{
		m_vampires[m_nVampires] = new Vampire(this, r, c);
		m_nVampires++;
		return true;
	}
}

bool Arena::addPlayer(int r, int c)
{
	if (!isPosInBounds(r, c))
		return false;

	// Don't add a player if one already exists
	if (m_player != nullptr)
		return false;

	// Don't add a player on a spot with a vampire
	if (numberOfVampiresAt(r, c) > 0)
		return false;

	m_player = new Player(this, r, c);
	return true;
}

void Arena::moveVampires()
{
	for (int i = 0; i < m_nVampires; i++)
	{
		m_vampires[i]->move();
		if ((m_vampires[i]->col() == m_player->col()) && (m_vampires[i]->row() == m_player->row()))
			m_player->setDead();
		if (m_vampires[i]->isDead())
		{
			delete m_vampires[i];
			m_nVampires--;
		}
	}
	m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
	return (r >= 1 && r <= m_rows && c >= 1 && c <= m_cols);
}

void Arena::checkPos(int r, int c, string functionName) const
{
	if (r < 1 || r > m_rows || c < 1 || c > m_cols)
	{
		cout << "***** " << "Invalid arena position (" << r << ","
			<< c << ") in call to " << functionName << endl;
		exit(1);
	}
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nVampires)
{
	if (nVampires < 0)
	{
		cout << "***** Cannot create Game with negative number of vampires!" << endl;
		exit(1);
	}
	if (nVampires > MAXVAMPIRES)
	{
		cout << "***** Trying to create Game with " << nVampires
			<< " vampires; only " << MAXVAMPIRES << " are allowed!" << endl;
		exit(1);
	}
	int nEmpty = rows * cols - nVampires - 1;  // 1 for Player
	if (nEmpty < 0)
	{
		cout << "***** Game created with a " << rows << " by "
			<< cols << " arena, which is too small too hold a player and "
			<< nVampires << " vampires!" << endl;
		exit(1);
	}

	// Create arena
	m_arena = new Arena(rows, cols);

	// Add player
	int rPlayer;
	int cPlayer;
	do
	{
		rPlayer = randInt(1, rows);
		cPlayer = randInt(1, cols);
	} while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
	m_arena->addPlayer(rPlayer, cPlayer);

	// Populate with vampires
	while (nVampires > 0)
	{
		int r = randInt(1, rows);
		int c = randInt(1, cols);
		if (r == rPlayer && c == cPlayer)
			continue;
		m_arena->addVampire(r, c);
		nVampires--;
	}
}

Game::~Game()
{
	delete m_arena;
}

string Game::takePlayerTurn()
{
	for (;;)
	{
		cout << "Your move (n/e/s/w/x or nothing): ";
		string playerMove;
		getline(cin, playerMove);

		Player* player = m_arena->player();
		int dir;

		if (playerMove.size() == 0)
		{
			if (recommendMove(*m_arena, player->row(), player->col(), dir))
				return player->move(dir);
			else
				return player->dropPoisonVial();
		}
		else if (playerMove.size() == 1)
		{
			if (tolower(playerMove[0]) == 'x')
				return player->dropPoisonVial();
			else if (decodeDirection(playerMove[0], dir))
				return player->move(dir);
		}
		cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
	}
}

void Game::play()
{
	m_arena->display("");
	Player* player = m_arena->player();
	if (player == nullptr)
		return;
	while (!player->isDead() && m_arena->vampireCount() > 0)
	{
		string msg = takePlayerTurn();
		m_arena->display(msg);
		if (player->isDead())
			break;
		m_arena->moveVampires();
		m_arena->display(msg);
	}
	if (player->isDead())
		cout << "You lose." << endl;
	else
		cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

  // Return a uniformly distributed random int from lowest to highest, inclusive
int randInt(int lowest, int highest)
{
	if (highest < lowest)
		swap(highest, lowest);
	static random_device rd;
	static default_random_engine generator(rd());
	uniform_int_distribution<> distro(lowest, highest);
	return distro(generator);
}

bool decodeDirection(char ch, int& dir)
{
	switch (tolower(ch))
	{
	default:  return false;
	case 'n': dir = NORTH; break;
	case 'e': dir = EAST;  break;
	case 's': dir = SOUTH; break;
	case 'w': dir = WEST;  break;
	}
	return true;
}

bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
	switch (dir)
	{
	case NORTH:
		if (r == 1)
			return false;
		else
		{
			r -= 1;
			return true;
		}
	case EAST:
		if (c == a.cols())
			return false;
		else
		{
			c += 1;
			return true;
		}
	case SOUTH:
		if (r == a.rows())
			return false;
		else
		{
			r += 1;
			return true;
		}
	case WEST:
		if (c == 1)
			return false;
		else
		{
			c -= 1;
			return true;
		}
	}

	return true;
}

// Recommend a move for a player at (r,c):  A false return means the
// recommendation is that the player should drop a poisoned blood vial and
// not move; otherwise, this function sets bestDir to the recommended
// direction to move and returns true.
//player moves before vampires
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
	// TODO:  Implement this function
	// Delete the following line and replace it with your code.
	int n = a.numberOfVampiresAt(r - 1, c);
	int e = a.numberOfVampiresAt(r, c + 1);
	int s = a.numberOfVampiresAt(r + 1, c);
	int w = a.numberOfVampiresAt(r, c - 1);
	int nn = a.numberOfVampiresAt(r - 2, c);
	int ne = a.numberOfVampiresAt(r - 1, c + 1);
	int nw = a.numberOfVampiresAt(r - 1, c - 1);
	int ee = a.numberOfVampiresAt(r, c + 2);
	int es = a.numberOfVampiresAt(r + 1, c + 1);
	int ss = a.numberOfVampiresAt(r + 2, c);
	int sw = a.numberOfVampiresAt(r + 1, c - 1);
	int ww = a.numberOfVampiresAt(r, c - 2);

	if (r == 1)
	{
		if (c == 1)
		{
			if (e == 0)
				bestDir = EAST;
			else if (s == 0)
				bestDir = SOUTH;
		}
		else if (c == a.cols())
		{
			if (w == 0)
				bestDir = WEST;
			else if (s == 0)
				bestDir = SOUTH;
		}
		else
		{
			if (s == 0 && ss == 0)
				bestDir = SOUTH;
			else if (w == 0 && ww == 0)
				bestDir = WEST;
			else if (e == 0 && ee == 0)
				bestDir = 0;
		}
		return true;
	}
	else if (r == a.rows())
	{
		if (c == 1)
		{
			if (e == 0)
				bestDir = EAST;
			else if (n == 0)
				bestDir = NORTH;
		}
		else if (c == a.cols())
		{
			if (w == 0)
				bestDir = WEST;
			else if (n == 0)
				bestDir = NORTH;
		}
		else
		{
			if (n == 0 && nn == 0)
				bestDir = NORTH;
			else if (w == 0 && ww == 0)
				bestDir = WEST;
			else if (e == 0 && ee == 0)
				bestDir = 0;
		}
		return true;
	}
	else if (c == 1)
	{
		if (n == 0 && nn == 0)
			bestDir = NORTH;
		else if (e == 0 && ee == 0)
			bestDir = EAST;
		else if (s == 0 && ss == 0)
			bestDir = SOUTH;
		return true;
	}
	else if (c == a.cols())
	{
		if (n == 0 && nn == 0)
			bestDir = NORTH;
		else if (s == 0 && ss == 0)
			bestDir = SOUTH;
		else if (w == 0 && ww == 0)
			bestDir = WEST;
		return true;
	}
	else if ((n != 0 && e != 0 && s != 0 && w != 0) && (nn != 0 && ne != 0 && nw != 0 && ee != 0 && es != 0 && ss != 0 && sw != 0 && ww != 0))
		return false;
	else if (n == 0 && e == 0 && s == 0 && w == 0)
		return false;
	else if (n == 0 && nn == 0 && ne == 0 && nw == 0)
	{
		bestDir = NORTH;
		return true;
	}
	else if (e == 0 && ee == 0 && ne == 0 && es == 0)
	{
		bestDir = EAST;
		return true;
	}
	else if (s == 0 && ss == 0 && es == 0 && sw == 0)
	{
		bestDir = SOUTH;
		return true;
	}
	else if (w == 0 && ww == 0 && nw == 0 && sw == 0)
	{
		bestDir = WEST;
		return true;
	}
	else
		return false;


	// Your replacement implementation should do something intelligent.
	// You don't have to be any smarter than the following, although
	// you can if you want to be:  If staying put runs the risk of a
	// vampire possibly moving onto the player's location when the vampires
	// move, yet moving in a particular direction puts the player in a
	// position that is safe when the vampires move, then the chosen
	// action is to move to a safer location.  Similarly, if staying put
	// is safe, but moving in certain directions puts the player in
	// danger of dying when the vampires move, then the chosen action should
	// not be to move in one of the dangerous directions; instead, the player
	// should stay put or move to another safe position.  In general, a
	// position that may be moved to by many vampires is more dangerous than
	// one that may be moved to by few.
	//
	// Unless you want to, you do not have to take into account that a
	// vampire might be poisoned and thus sometimes less dangerous than one
	// that is not.  That requires a more sophisticated analysis that
	// we're not asking you to do.
}

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////


int main()
{
	// Create a game
	// Use this instead to create a mini-game:   Game g(3, 5, 2);
	Game g(10, 12, 40);

	// Play the game
	g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD upperLeft = { 0, 0 };
	DWORD dwCharsWritten;
	FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
		&dwCharsWritten);
	SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
	static const char* term = getenv("TERM");
	if (term == nullptr || strcmp(term, "dumb") == 0)
		cout << endl;
	else
	{
		static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
		cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
	}
}

#endif

