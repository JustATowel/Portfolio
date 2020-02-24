#include "pch.h"
#include "Bullet.h"


void Bullet::Update()
{	
	SetYPos(GetYPos() - 1);
	if (GetYPos() <= 1)
	{
		isShooting = false;
	}
}

void Bullet::Render()const
{	
	if (isShooting)
	{
		BaseObject::Render();
	}
}

Bullet::Bullet()
{
	isShooting = false;
	SetSymbol("|");
}


Bullet::~Bullet()
{
}
