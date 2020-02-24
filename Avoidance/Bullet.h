#pragma once
#include "BaseObject.h"

class Bullet :
	public BaseObject
{	
public:
	bool isShooting = false;	
	void Update();
	void Render()const;
	Bullet();
	~Bullet();
};

