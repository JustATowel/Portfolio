#pragma once
#include "BaseObject.h"
class Player :
	public BaseObject
{
public:	
	bool isAlive;
	int lives;
	void Update();	
	Player();
	~Player();
};

