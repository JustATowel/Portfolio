#pragma once
#include "Console.h"
using namespace System;
class Game;
class HUD : public BaseObject
{
	Game* g;
	bool playerAlive = true;
public:
	HUD(Game* thegame)
	{
		g = thegame;
	}

	virtual void Render()const
	{
		if (playerAlive)
		{
			Console::SetCursorPosition(0, 0);
			cout << "SCORE: " << g->GetScore();
			Console::SetCursorPosition(15, 0);
			cout << "LIVES: " << g->GetLives();
		}		
	}
};