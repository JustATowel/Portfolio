#pragma once
#include "ComputerMove.h"
#include "Board.h"


class ComputerAI
{
private:



public:

	ComputerAI() {};
	~ComputerAI() {};

	ComputerMove RunAI(choices next_player, GameBoard& _board, int difficulty);

	ComputerMove GetBestMove(choices _player, GameBoard& _board, int difficulty);

	void CollectValidMoves(std::vector<ComputerMove>& _vec, GameBoard& _board);

	choices SwitchPlayer(choices _sym);

};