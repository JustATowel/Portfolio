#include "pch.h"
#include "Enemy.h"


Enemy::Enemy()
{	
	int randomX = rand() % 101;
	SetSymbol("O");
	SetYPos(rand() % 50);
	SetXPos(randomX);	
}

bool Enemy::Awake()
{
	return isAwake;
}

void Enemy::Update()
{	
	SetYPos(GetYPos() + 1);	
	if (GetYPos() >= 50)
	{
		int randomX = rand() % 101;		
		SetXPos(randomX);
		SetYPos(1);
		isAwake = true;
	}	
}

Enemy::~Enemy()
{
}

void Enemy::Render()const
{
	if (isAwake)
		BaseObject::Render();
}