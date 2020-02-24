#include "pch.h"
#include "Player.h"


Player::Player()
{
	SetXPos(50);
	SetYPos(45);
}

void Player::Update()
{
	if (GetAsyncKeyState(VK_LEFT)) 
	{
		SetXPos(GetXPos() - 1); 
		if (GetXPos() < 0)
		{
			SetXPos(100);
		}
	}
	if (GetAsyncKeyState(VK_RIGHT)) 
	{
		SetXPos(GetXPos() + 1);
		if (GetXPos() > 100)
		{
			SetXPos(0);
		}
	}
}

Player::~Player()
{
}
