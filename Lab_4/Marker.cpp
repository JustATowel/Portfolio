#include "pch.h"
#include "Marker.h"
#include "Console.h"
using namespace System;


Marker::Marker()
{
	SetXPos(0);
	SetYPos(2);
}

int Marker::GetXPos() { return xPos; }
int Marker::GetYPos() { return yPos; }
void Marker::SetXPos(int x) { xPos = x; }
void Marker::SetYPos(int y) { yPos = y; }
void Marker::Update()
{
	if (GetAsyncKeyState(VK_LEFT))
	{
		SetXPos(GetXPos() - 1);
		if (GetXPos() < 0)
		{
			SetXPos(31);
		}
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		SetXPos(GetXPos() + 1);
		if (GetXPos() > 31)
		{
			SetXPos(0);
		}
	}
}
void Marker::Render()
{
	Console::SetCursorPosition(xPos, yPos);
	cout << image;
}
Marker::~Marker()
{
}
