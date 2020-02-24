#include "pch.h"
#include "BitString.h"
#include "Marker.h"
#include "Console.h"
using namespace System;



int main()
{
	bool isPlaying = true;
	BitString b;
	Marker m;
	b.SetMarker(&m);
	while (isPlaying)
	{
		Console::Lock(true);
		system("cls");
		Console::CursorVisible(0);
		b.RenderNum();
		m.Render();
		m.Update();
		b.UpdateNum();
		Console::Lock(false);
		Sleep(20);
	}
	
	// while loop
	
	
	
}

