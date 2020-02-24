#pragma once
class Marker;
class BitString
{
	unsigned int original = 0;
	int xPos;
	int yPos;
	Marker* mark;
public:
	BitString();
	void TurnOff();
	void TurnOn();
	void Toggle();
	void Negate();
	void LeftShift();
	void RightShift();
	void UpdateNum();
	void RenderNum();
	int GetNum();
	void SetNum(unsigned int);	
	void SetMarker(Marker*);
	~BitString();
};

