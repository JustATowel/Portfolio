#pragma once

class BaseObject
{
	int xPos;
	int yPos;
	char* image = nullptr;	
	int lives = 3;
public:		
	const char* GetSymbol();
	void SetSymbol(const char*);
	int GetXPos();
	int GetYPos();
	void SetXPos(int);
	void SetYPos(int);	
	virtual void Update();
    virtual void Render()const;
	BaseObject();
	~BaseObject();
};

