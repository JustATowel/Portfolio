#include "AI.h"

ComputerMove ComputerAI::RunAI(choices next_player, GameBoard& _board, int difficulty)
{
	ComputerMove next_move = GetBestMove(next_player, _board, difficulty);

	return next_move;
}

ComputerMove ComputerAI::GetBestMove(choices _player, GameBoard& _board, int difficulty)
{
	ComputerMove best_move;
	GameBoard _gb;
	std::vector<ComputerMove> valid_moves;

	if (_board.GetValidMoveCount() > 0)
	{
		CollectValidMoves(valid_moves, _board);
		
		for (int moves_index = 0; moves_index < valid_moves.size(); ++moves_index)
		{
			_gb.CopyBoard(_board, _gb);
			_gb.MakeMove(_player, valid_moves[moves_index].row_index, valid_moves[moves_index].column_index);

			if (_gb.IsGameOver(_player) || difficulty == 0)
			{
				valid_moves[moves_index].rank = _gb.Evaluate() - difficulty;
			}			
			else
				valid_moves[moves_index].rank = GetBestMove(SwitchPlayer(_player), _gb, difficulty - 1).rank;
			

			if ((valid_moves[moves_index].rank < best_move.rank || (best_move.column_index > 3 || best_move.row_index > 3 || best_move.column_index < 0 || best_move.row_index < 0)) && _player == 1)
			{
				best_move = valid_moves[moves_index];
			}
			else if ((valid_moves[moves_index].rank > best_move.rank || (best_move.column_index > 3 || best_move.row_index > 3 || best_move.column_index < 0 || best_move.row_index < 0)) && _player == 2)
			{
				best_move = valid_moves[moves_index];
			}
		}

	}
		
	return best_move;
}

void ComputerAI::CollectValidMoves(std::vector<ComputerMove>& _vec, GameBoard& _board)
{
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			if (_board.board_spots[x][y] == 0)
			{
				ComputerMove _move;
				_move.row_index = x;
				_move.column_index = y;
				_vec.push_back(_move);
			}
		}
	}
}



choices ComputerAI::SwitchPlayer(choices _sym)
{
	if (_sym == X)
	{
		return O;
	}
	else if (_sym == O)
	{
		return X;
	}
		
}