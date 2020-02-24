#pragma once
#include "BaseObject.h"
class Enemy :
	public BaseObject
{
	bool isAwake = false;	
public:	
	bool Awake();
	inline void SetAwake(bool a) { isAwake = a; }
	void Update();
	void Render()const;
	Enemy();
	~Enemy();
};

