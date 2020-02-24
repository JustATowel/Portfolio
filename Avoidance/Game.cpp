#include "pch.h"
#include "Game.h"
#include "Console.h"
#include "Player.h"
#include "Enemy.h"
#include "BaseObject.h"
#include "HUD.h"
using namespace System;
Game::Game()
{	
	Score s;
	Console::SetWindowSize(100, 50);
	Console::SetBufferSize(100, 50);
	Player* p = new Player();
	p->SetSymbol("^");	
	objects.push_back(p);	
	objects.push_back(new HUD(this));
		for (int i = 0; i < 100; ++i)
		{			
			Enemy* e = new Enemy();			
			objects.push_back(e);			
		}
		for (int i = 0; i < 100; ++i)
		{
			Bullet* b = new Bullet();			
			
			bullets.push_back(b);
		}
	}

Game::~Game()
{
	for (int i = 0; i < objects.size(); ++i)
	{
		delete objects[i];
	}
	for (int i = 0; i < bullets.size(); ++i)
	{
		delete bullets[i];
	}
}

int Game::GetScore()
{
	 return score; 
}

int Game::GetLives()
{
	return lives;
}

void Game::CollideCheck()
{
	for (int i = 0; i < objects.size(); ++i)
	{
		Player* pCollide = dynamic_cast<Player*>(objects[i]);
		if (pCollide)
		{
			int pX = pCollide->GetXPos();
			int pY = pCollide->GetYPos();
			for (int j = 0; j < objects.size(); ++j)
			{
				if (pCollide == objects[j])
					continue;
				Enemy* eCollide = dynamic_cast<Enemy*>(objects[j]);
				if (eCollide)
				{
					int eX = eCollide->GetXPos();
					int eY = eCollide->GetYPos();
					if (pX == eX && pY == eY)
					{
						if (eCollide->Awake() == true)
						lives--;	
						if (lives == 0)
						{
							

							stateMachine = EndGame;

						}
						break;
					}
				}
			}
			
			break;
		}
	}
}

void Game::BulletCollide()
{
	for (int i = 0; i < objects.size(); ++i)
	{
		Enemy* enemCollide = dynamic_cast<Enemy*>(objects[i]);
		if (enemCollide)
		{
			int enemX = enemCollide->GetXPos();
			int enemY = enemCollide->GetYPos();
			for (int j = 0; j < bullets.size(); ++j)
			{
				if (bullets[j]->isShooting == true)
				{
					int bullX = bullets[j]->GetXPos();
					int bullY = bullets[j]->GetYPos();
					if (enemX == bullX && enemY == bullY)
					{
						bullets[j]->isShooting = false;
						objects[i]->SetXPos(rand()%101);
						objects[i]->SetYPos(1);
						score += 1;
						break;
					}
				}
			}
			
		}
	}
}

void Game::PlayGame()
{	
	for (int i = 0; i < objects.size(); ++i)
	{
		objects[i]->Update();
		objects[i]->Render();			
	}	
	if (GetAsyncKeyState(VK_SPACE))
	{		
			for (int j = 0; j < objects.size(); ++j)
			{
				Player* pPos = dynamic_cast<Player*>(objects[j]);
				if (pPos)
				{	
					int pX = pPos->GetXPos();
					int pY = pPos->GetYPos();
					for (int i = 0; i < bullets.size(); ++i)
					{
					if (bullets[i]->isShooting == false)
					{
						bullets[i]->isShooting = true;
						bullets[i]->SetXPos(pX);
						bullets[i]->SetYPos(pY);
							break;
					}
				}
			}
			
		}
	}
	for (int i = 0; i < bullets.size(); ++i)
	{
		bullets[i]->Update();
		bullets[i]->Render();
	}	
	CollideCheck();
	BulletCollide();	
}

void Game::GameMenu()
{
	
	Console::DrawBox(7, 5, 85, 40, 1);
	Console::SetCursorPosition(39, 15);	
	cout << "Dodgeball Practice!!!";
	Console::SetCursorPosition(44, 25);
	cout << "Ctrl: Play";
	Console::SetCursorPosition(38, 27);
	cout << "RightShift: HighScores";
	Console::SetCursorPosition(36, 29);
	cout << "LeftShift: HighScores(Text)";
	Console::SetCursorPosition(44, 31);
	cout << "Esc: Quit";
	Console::CursorVisible(false);
	
	if (GetAsyncKeyState(VK_RCONTROL) || GetAsyncKeyState(VK_LCONTROL)) { stateMachine = Play; }
	if (GetAsyncKeyState(VK_RSHIFT)) { stateMachine = PrintScores; }
	if (GetAsyncKeyState(VK_ESCAPE)) { stateMachine = Quit; }
	if (GetAsyncKeyState(VK_LSHIFT)) { stateMachine = PrintTextScores; }
	
	
}

void Game::QuitGame()
{
	isPlaying = false;
}

void Game::EndScreen()
{	
	dynamic_cast<Player*>(objects[0])->SetXPos(50);
	dynamic_cast<Player*>(objects[0])->SetYPos(45);
	for (int i = 2; i < objects.size(); ++i)
	{		
		dynamic_cast<Enemy*>(objects[i])->SetAwake(false);
	}
	for (int i = 0; i < bullets.size(); ++i)
	{
		bullets[i]->isShooting = false;
	}
	lives = 3;
	score = 0;
	SaveScoreToFileTB();
	Console::FlushKeys();
}

void Game::PrintScoreFileT()
{
	Console::DrawBox(7, 5, 85, 40, 1);
	vector<Score> scores;
	Score s;
	int highscore;
	int x = 43, y = 15;

	fin.open("HighScores.txt", ios_base::in);
	if (fin.is_open())
	{
		while (true)
		{
			if (fin.eof())
				break;
			fin.getline(s.name, INT_MAX, '\t');		

			fin >> s.pscore;

			fin.ignore(INT_MAX, '\n');	
			Console::SetCursorPosition(x, y);
			cout << s.name << "\t" << s.pscore;
			y += 2;
			scores.push_back(s);
		}
		fin.close();		
	}
	Console::SetCursorPosition(34, y + 5);
	cout << "Press Enter to return to menu";
	if (GetAsyncKeyState(VK_RETURN)) { stateMachine = Menu; }
	
	

	
}

void Game::SaveScoreToFileTB()
{
	Console::DrawBox(7, 5, 85, 40, 1);
	Console::SetCursorPosition(39, 15);
	vector<Score> scores;
	Score s;
	cout << "NEW HIGHSCORE!" << endl;
	Console::SetCursorPosition(30, 17);
	cout << "Type your name to save your highscore: " << endl;
	Console::SetCursorPosition(45, 19);
	cin >> s.name;
	cin.clear();
	cin.ignore();
	s.pscore = score;
	fout.open("HighScores.txt", ios_base::out | ios_base::app);
	if (fout.is_open())
	{ 
		fout << "\n" << s.name << "\t" << s.pscore;
		fout.close();
	}
	bout.open("HighScores.bin", ios_base::out | ios_base::binary | ios_base::app);
	if (bout.is_open())
	{
		bout.write((char*)&s, sizeof(s));

		bout.close();
	}
	stateMachine = Menu;
	
}

void Game::PrintScoreFileB()
{	
	Console::DrawBox(7, 5, 85, 40, 1);
	int x = 43, y = 15;
	vector<Score> scores;	
	bin.open("HighScores.bin", ios_base::in | ios_base::binary);
	if (bin.is_open())
	{		
		bin.seekg(0, ios_base::end);
		int count = (int)bin.tellg();
		count /= sizeof(Score);
		bin.seekg(0, ios_base::beg);
		scores.resize(count);		
		bin.read((char*)&scores[0], scores.size() * sizeof(Score));
		bin.close();
	}
	for (int i = 0; i < (int)scores.size(); ++i, y+=2)
	{
		Console::SetCursorPosition(x, y);
		cout << scores[i].name << "\t" << scores[i].pscore << '\n';
	}
	Console::SetCursorPosition(34, y + 5);
	cout << "Press Enter to return to menu";
	if (GetAsyncKeyState(VK_RETURN)) { stateMachine = Menu; }	
}

void Game::RunGame()
{   
	while (isPlaying)
	{
		Console::Lock(true);
		system("cls");		
		switch (stateMachine)
		{
		case Menu:
			GameMenu();			
			break;
		case Play:			
			PlayGame();
			break;
		case PrintScores:
			PrintScoreFileB();
			break;
		case Quit:
			QuitGame();
			break;			
		case EndGame:		
			Console::Lock(false);
			EndScreen();
			break;
		case PrintTextScores:
			PrintScoreFileT();
			break;
		}
		Console::Lock(false);
		Sleep(20);
	}
	
}


