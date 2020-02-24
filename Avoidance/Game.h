#pragma once
#include "BaseObject.h"
#include "Bullet.h"

class Game
{
	enum gameStates { Menu, Play, PrintScores, Quit, EndGame, PrintTextScores };
	gameStates stateMachine = Menu;
	bool isPlaying = true;
	vector<BaseObject*> objects;
	vector<Bullet*> bullets;	
	int score = 0;
	int lives = 3;
	fstream fout;
	fstream fin;
	fstream bout;
	fstream bin;
public:		
	void PlayGame();	
	void GameMenu();
	void QuitGame();
	void RunGame();
	void PrintScoreFileT();
	void SaveScoreToFileTB();
	void PrintScoreFileB();	
	void BulletCollide();
	void EndScreen();
	Game();
	~Game();
	int GetScore();
	int GetLives();
	void CollideCheck();
};
struct Score
{
	char name[32];
	int pscore;
};
