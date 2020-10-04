#pragma once

class ComputerMove
{
private:

public:
	int rank;
	int row_index;
	int column_index;

	ComputerMove() { rank = 0; };
	ComputerMove(int _row, int _column) 
	{
		row_index = _row;
		column_index = _column;
		rank = 0;
	};
	~ComputerMove() {};

};