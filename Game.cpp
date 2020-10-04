#include "Game.h"


Game::Game()
{
	g_state = MENU;
}

Game::~Game()
{

}

//this function runs through the game loop, from menu to win/loss, until the player decides to stop
void Game::Run()
{
	while (g_state != QUIT)
	{
		switch (g_state)
		{
		case MENU:
		{			
			while (g_state == MENU)
			{
				g_state = MainMenu();				
			}	
			m_board.ResetBoard();
			system("CLS");
		}
		break;
		case PLAYER_TURN:
		{
			bool pick = false;
			Draw();
			while (!pick)
			{
				pick = GetPick();
			}
			Draw();

			if (m_board.IsGameOver(X))
			{
				g_state = WIN;
			}
			else if (m_board.GetValidMoveCount() == 0)
			{
				g_state = TIE;
			}
			else
				g_state = OPPONENT_TURN;
		}
		break;
		case OPPONENT_TURN:
		{
			Draw();
			OpponentsTurn();
			Draw();
			if (m_board.IsGameOver(O))
			{
				g_state = LOSE;
			}
			else if (m_board.GetValidMoveCount() == 0)
			{
				g_state = TIE;
			}
			else
				g_state = PLAYER_TURN;
		}
		break;
		case WIN:
		{
			
			while (g_state == WIN)
			{
				g_state = PlayerWin();
			}
			
			system("CLS");
		}
		break;
		case LOSE:
		{
		
			while (g_state == LOSE)
			{
				g_state = PlayerLoss();
			}
			
			system("CLS");
		}
		break;
		case TIE:
		{
			
			while (g_state == TIE)
			{
				g_state = PlayerTie();
			}
			system("CLS");
		}
		break;
		}
	}
	Exit();
	
}

//this function will delete anything needed and handle loose ends before shut down
int Game::Exit()
{
	exit(0);
}

void Game::Draw()
{
	int cell_num = 1;
	COORD coord;
	//draws the vertical lines
	for (int x = 0; x < 2; ++x)
	{
		coord.X = (x + 1) * 5;

		for (int y = 3; y < 5 * 3; ++y)
		{
			coord.Y = y;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			std::cout << "|";
		}
	}
	//draws the horizontal lines
	for (int y = 0; y < 2; ++y)
	{
		coord.Y = (y + 1) * 5;

		for (int x = 3; x < 5 * 3; ++x)
		{
			coord.X = x;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			std::cout << "-";
		}
	}
	//draws the numbers in the grid
	for (int y = 0; y < 3; ++y)
	{
		coord.Y = (y * 5 + 5) - 2;

		for (int x = 0; x < 3; ++x)
		{
			coord.X = x * 5 +5 - 2;

			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			
			
			if (m_board.board_spots[x][y] != 0)
			{
				std::cout << symbols[m_board.board_spots[x][y] - 1];
			}
			else
			std::cout << cell_num;
			++cell_num;
		}
		
	}
}

bool Game::GetPick()
{
	COORD coord;
	coord.X = 0;
	coord.Y = 20;
	int choice = 0;
	int curr_num = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	std::cout << "\33[2K";
	std::cout << "Input the number cell you would like to play :  ";
	
	if (!(std::cin >> choice))
	{
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		choice = 0;
	}

	if (choice < 1 || choice > 9)
	{
		std::cout << "\33[2K";
		std::cout << "Not a valid choice.";
		Sleep(750);
		std::cout << "\33[2K";
		std::cin.clear();
		return false;
	}
	
	
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			if (curr_num+1 != choice)
			{
				++curr_num;
				continue;
			}
			if (m_board.board_spots[x][y] >= 1)
			{
				std::cout << "\33[2K";
				std::cout << "Not a valid choice.";
				Sleep(750);
				std::cout << "\33[2K";
				std::cin.clear();
				return false;
			}
			else
			{
				m_board.board_spots[x][y] = X;
				return true;  
			}

		}
	}	
}

void Game::OpponentsTurn()
{
	COORD coord;
	coord.X = 0;
	coord.Y = 20;
	int choice = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	std::cout << "\33[2K";
	std::cout << "Opponent's Move, Please Wait...";
	Sleep(750);
	//pick move algorithm
	ComputerMove _move = ai.RunAI(O, m_board, diff);
	//if the move returned is taken already there are no valid moves and it is a tie
	m_board.board_spots[_move.row_index][_move.column_index] = O;
	
}

gamestate Game::PlayerLoss()
{
	COORD coord;
	coord.X = 0;
	coord.Y = 20;
	int choice = 0;
	int curr_num = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	std::cout << "\33[2K";
	std::cout << "YOU LOSE!" << '\n' << "Press 1 to play again." << '\n' << "Press 2 to quit. " << std::endl;

	if (!(std::cin >> choice))
	{
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		choice = 0;
	}

	if (choice < 1 || choice > 2)
	{
		std::cout << "\33[2K";
		std::cout << "Not a valid choice.";
		Sleep(750);
		std::cout << "\33[2K";
		std::cin.clear();
		return LOSE;
	}
	if (choice == 1)
	{
		return MENU;
	}
	else if (choice == 2)
	{
		return QUIT;
	}
}

gamestate Game::PlayerWin()
{
	COORD coord;
	coord.X = 0;
	coord.Y = 20;
	int choice = 0;
	int curr_num = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	std::cout << "\33[2K";
	std::cout << "YOU WIN!" << '\n' << "Press 1 to play again." << '\n' << "Press 2 to quit. " << std::endl;

	if (!(std::cin >> choice))
	{
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		choice = 0;
	}

	if (choice < 1 || choice > 2)
	{
		std::cout << "\33[2K";
		std::cout << "Not a valid choice.";
		Sleep(750);
		std::cout << "\33[2K";
		std::cin.clear();
		return WIN;
	}
	if (choice == 1)
	{
		return MENU;
	}
	else if (choice == 2)
	{
		return QUIT;
	}
}

gamestate Game::PlayerTie()
{
	COORD coord;
	coord.X = 0;
	coord.Y = 20;
	int choice = 0;
	int curr_num = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	std::cout << "\33[2K";
	std::cout << "YOU TIE!" << '\n' << "Press 1 to play again." << '\n' << "Press 2 to quit. " << std::endl;
	
	if (!(std::cin >> choice))
	{
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		choice = 0;
	}

	if (choice < 1 || choice > 2)
	{
		std::cout << "\33[2K";
		std::cout << "Not a valid choice.";
		Sleep(750);
		std::cout << "\33[2K";
		std::cin.clear();
		return TIE;
	}
	if (choice == 1)
	{
		return MENU;
	}
	else if (choice == 2)
	{
		return QUIT;
	}
}

gamestate Game::MainMenu()
{
	
	COORD coord;
	coord.X = 0;
	coord.Y = 1;
	int choice = 0;
	int curr_num = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	std::cout << "\33[2K";
	std::cout << "TIC-TAC-TOE" << "\n\n\n" << "Choose a difficulty to begin: " << '\n' << "1 - Easy" << '\n' << "2 - Hard" << '\n' << "3 - Quit" << std::endl;

	if (!(std::cin >> choice))
	{
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		choice = 0;
	}

	if (choice < 1 || choice > 3)
	{
		std::cout << "\33[2K";
		std::cout << "Not a valid choice.";
		Sleep(750);
		std::cout << "\33[2K";
		std::cin.clear();
		return MENU;
	}
	if (choice == 1)
	{
		diff = EASY;		
		return PLAYER_TURN;
	}
	else if (choice == 2)
	{
		diff = HARD;		
		return PLAYER_TURN;
	}	
	else if (choice == 3)
	{
		return QUIT;
	}
}

