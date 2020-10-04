#include "Board.h"

GameBoard::GameBoard()
{
}

GameBoard::~GameBoard()
{
}

int GameBoard::GetValidMoveCount()
{
	int count = 0;
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			if (board_spots[x][y] != 1 && board_spots[x][y] != 2)
			{
				++count;
			}
		}
	}
	return count;
}

void GameBoard::CopyBoard(GameBoard& _original, GameBoard& _new)
{
	
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			_new.board_spots[x][y] = _original.board_spots[x][y];
		}
	}
	
}

bool GameBoard::IsGameOver(choices _player)
{
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			if (board_spots[x][y] == _player)
			{
				//horizontal victory
				if (x + 1 <= 2 && x + 2 <= 2)
				{
					if (board_spots[x][y] == board_spots[x + 1][y] && board_spots[x][y] == board_spots[x + 2][y])
					{
						return true;
					}
				}
				//vertical victory
				if (y + 1 <= 2 && y + 2 <= 2)
				{
					if (board_spots[x][y] == board_spots[x][y + 1] && board_spots[x][y] == board_spots[x][y + 2])
					{
						return true;
					}
				}
				//left to right diagonal victory
				if (y + 1 <= 2 && y + 2 <= 2 && x + 1 <= 2 && x + 2 <= 2)
				{
					if (board_spots[x][y] == board_spots[x + 1][y + 1] && board_spots[x][y] == board_spots[x + 2][y + 2])
					{
						return true;
					}
				}
				//right to left diagonal victory
				if (y + 1 <= 2 && y + 2 <= 2 && x - 1 <= 2 && x - 2 <= 2)
				{
					if (board_spots[x][y] == board_spots[x - 1][y + 1] && board_spots[x][y] == board_spots[x - 2][y + 2])
					{
						return true;
					}
				}

			}
		}
	}

	return false;
}

void GameBoard::MakeMove(choices _player, int _row, int _column)
{
	board_spots[_row][_column] = _player;
}

void GameBoard::ResetBoard()
{
	for (int x = 0; x < 3; ++x)
	{
		for (int y = 0; y < 3; ++y)
		{
			board_spots[x][y] = 0;
		}
	}
}

int GameBoard::Evaluate()
{
	if (IsGameOver(X))
	{
		return -10;
	}
	else if (IsGameOver(O))
	{
		return 10;
	}
}
