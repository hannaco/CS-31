#include "grid.h"

void plotHorizontalLine(int r, int c, int distance, char ch);
void plotVerticalLine(int r, int c, int distance, char ch);
void plotRectangle(int r, int c, int height, int width, char ch);

int main()
{
	setSize(20, 30);
	plotRectangle(1, 1, 3, 6, '=');
	draw();
}

void plotHorizontalLine(int r, int c, int distance, char ch)
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

void plotVerticalLine(int r, int c, int distance, char ch)
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

void plotRectangle(int r, int c, int height, int width, char ch)
{
	if (height < 0 || width < 0)
		return;
	int bottomLeft = r + height;
	int topRight = c + width;
	plotVerticalLine(r, c, height, ch);
	plotVerticalLine(r, topRight, height, ch);
	plotHorizontalLine(r, c, width, ch);
	plotHorizontalLine(bottomLeft, c, width, ch);
}

