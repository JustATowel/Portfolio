
#include "pch.h"
#include "Game.h"



int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(-1); //Important!
	srand((unsigned int)time(0));
	Game g;
	g.RunGame();
	return 0;
	
}

