#include "pch.h"
#include "BitString.h"
#include "Console.h"
#include "Marker.h"
#define bigbit 2147483648
using namespace System;

BitString::BitString()
{
}
void BitString::UpdateNum()
{
	if (GetAsyncKeyState(VK_F1))
	{
		TurnOn();
	}
	if (GetAsyncKeyState(VK_F2))
	{
		Toggle();
	}
	if (GetAsyncKeyState(VK_F3))
	{
		TurnOff();
	}
	if (GetAsyncKeyState(VK_F4))
	{
		Negate();
	}
	if (GetAsyncKeyState(VK_LSHIFT))
	{
		LeftShift();
	}
	if (GetAsyncKeyState(VK_RSHIFT))
	{
		RightShift();
	}
	
}
void BitString::RenderNum()
{
	unsigned int x = GetNum();
	int y = 31;

	cout << "Bits:\t\t Current Number: " << original << endl;
	for (int i = 0; i < 32; i++)
	{
		if (i == mark->GetXPos())
		{
			Console::ForegroundColor(Green);
			
		}
		if (x >= pow(2, y))
		{
			cout << "1";
			
			x = (x - pow(2, y));
		}
		else
		{
			cout << "0";
			
		}
		Console::ResetColor();
		y--;
	}

}
void BitString::SetMarker(Marker* _mark)
{
	mark = _mark;
}
void BitString::TurnOff()
{
	original &= ~(bigbit >> mark->GetXPos());
}
void BitString::TurnOn()
{
	original |= (bigbit >> mark->GetXPos());
}
void BitString::Toggle()
{
	original ^= (bigbit >> mark->GetXPos());
}
void BitString::Negate()
{
	original = ~original;
}
void BitString::LeftShift()
{
	original = original << 1;
}
void BitString::RightShift()
{
	original = original >> 1;
}
int BitString::GetNum()
{
	return original;
}
void BitString::SetNum(unsigned int num)
{
	original = num;
}

BitString::~BitString()
{
}
