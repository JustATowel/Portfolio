#pragma once
#include "Globals.h"

class GameBoard
{
private:


public:

	int board_spots[3][3] = { 0 };
	int valid_move_count = 9;

	GameBoard();
	~GameBoard();
	int GetValidMoveCount();
	void CopyBoard(GameBoard& _original, GameBoard& _new);
	bool IsGameOver(choices _player);
	void MakeMove(choices _player, int _row, int _column);
	void ResetBoard();
	int Evaluate();
};