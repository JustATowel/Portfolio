#include "pch.h"
#include "BaseObject.h"
#include "Console.h"
using namespace System;

BaseObject::BaseObject()
{
}

void BaseObject::Update()
{
}

void BaseObject::Render()const
{	
	Console::SetCursorPosition(xPos, yPos);
	cout << image;
}

const char* BaseObject::GetSymbol()
{
	return image;
}

void BaseObject::SetSymbol(const char* sym)
{
	delete[]image;
	int length = strlen(sym) + 1;
	image = new char[length];
	strcpy_s(image, length, sym);	
}

int BaseObject::GetXPos()
{
	return xPos;
}

int BaseObject::GetYPos()
{
	return yPos;
}

void BaseObject::SetXPos(int x)
{
	xPos = x;
}

void BaseObject::SetYPos(int y)
{
	yPos = y;
}

BaseObject::~BaseObject()
{
	delete[] image;
}
