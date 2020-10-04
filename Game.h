#pragma once
#include "AI.h"

//this class will be in charge of controlling the game loop and win/loss conditions


class Game
{

private:

	GameBoard m_board;
	gamestate g_state;
	ComputerAI ai;
	difficulties diff;
	char symbols[2] = {'X', 'O' };

	void Draw();
	bool GetPick();
	void OpponentsTurn();
	gamestate PlayerLoss();
	gamestate PlayerWin();
	gamestate PlayerTie();
	gamestate MainMenu();
	
	


public:
	Game();
	~Game();

	void Run();
	int Exit();

};