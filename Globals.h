#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>

enum gamestate
{
	PLAYER_TURN,
	OPPONENT_TURN,
	WIN,
	LOSE,
	QUIT,
	TIE,
	MENU
};

enum choices
{
	X = 1,
	O

};

enum difficulties
{
	EASY = 1,
	MEDIUM,
	HARD = 10,
};