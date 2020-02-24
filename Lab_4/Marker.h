#pragma once
class Marker
{
	int xPos;
	int yPos;
	char image = '^';
public:
	int GetXPos();
	int GetYPos();
	void SetXPos(int);
	void SetYPos(int);
	void Update();
	void Render();
	Marker();
	~Marker();
};

