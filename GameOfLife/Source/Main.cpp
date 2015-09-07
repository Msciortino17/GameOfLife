/****************************************************
* Class:			Main.cpp
* Author:			Michael Sciortino
* Date Created:		09/06/15
* Last Modified:	09/06/15
* Purpose:			Entry point for program
*****************************************************/

#include <iostream>>
#include "Game.h"
#include <ctime>

int main()
{
	srand((unsigned int)time(nullptr));

	Game * game = new Game;
	game->Initialize(30, 15);

	while (game->GetRunning())
	{
		game->Update();
	}

	game->Terminate();
	delete game;
	game = nullptr;

	std::cout << "\n\n";
	system("pause");
	return 0;
}