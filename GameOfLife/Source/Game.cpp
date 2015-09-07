/****************************************************
* Class:			Game.cpp
* Author:			Michael Sciortino
* Date Created:		09/06/15
* Last Modified:	09/07/15
* Purpose:			Central hub for all things related to gameplay
*****************************************************/

#include "Game.h"
#include "InputManager.h"
#include <iostream>

void Game::Initialize(int nWidth, int nHeight)
{
	// Store default values
	m_bRunning = true;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_fTimer = 0.0f;
	m_fUpdateRate = 0.75f;
	m_nCursorX = 0;
	m_nCursorY = 0;
	m_nGeneration = 0;
	m_bSimulating = false;

	// Set up grid
	m_bGrid = new bool*[m_nWidth];
	m_bBufferGrid = new bool*[m_nWidth];
	for (int nX = 0; nX < m_nWidth; ++nX)
	{
		m_bGrid[nX] = new bool[m_nHeight];
		m_bBufferGrid[nX] = new bool[m_nHeight];
		for (int nY = 0; nY < m_nHeight; ++nY)
		{
			m_bGrid[nX][nY] = false;
			m_bBufferGrid[nX][nY] = false;
		}
	}

	ResetGrid();
	DisplayGrid();

	// Setup input
	InputManager * pInput = InputManager::GetInstance();
	pInput->Initialize(GetConsoleWindow());

}

void Game::Terminate()
{
	// Clean up grid
	delete[] m_bGrid;
	delete[] m_bBufferGrid;

	// Clean up input
	InputManager * pInput = InputManager::GetInstance();
	pInput->Terminate();

}

void Game::Update()
{
	// Calculate delta time
	m_xTime.Signal();
	m_fDeltaTime = (float)m_xTime.Delta();

	// Update the menu options
	UpdateMenu();

	// Update the grid at a set rate
	m_fTimer += m_fDeltaTime;
	if (m_fTimer > m_fUpdateRate)
	{
		m_fTimer = 0.0f;
		if (m_bSimulating)
		{
			m_nGeneration++;
			UpdateGrid();
		}

		// Display the results
		DisplayGrid();
	}

}

void Game::UpdateMenu()
{
	// Input reference
	InputManager * pInput = InputManager::GetInstance();
	pInput->Update();

	// Exit if pressed escape
	if (pInput->IsKeyPressed(Key::Escape))
	{
		SetRunning(false);
	}

	// Move cursor
	if (pInput->IsKeyPressed(Key::Left))
	{
		m_nCursorX--;
		if (m_nCursorX < 0)
		{
			m_nCursorX = m_nWidth - 1;
		}
		DisplayGrid();
	}
	if (pInput->IsKeyPressed(Key::Right))
	{
		m_nCursorX++;
		if (m_nCursorX >= m_nWidth)
		{
			m_nCursorX = 0;
		}
		DisplayGrid();
	}
	if (pInput->IsKeyPressed(Key::Up))
	{
		m_nCursorY--;
		if (m_nCursorY < 0)
		{
			m_nCursorY = m_nHeight - 1;
		}
		DisplayGrid();
	}
	if (pInput->IsKeyPressed(Key::Down))
	{
		m_nCursorY++;
		if (m_nCursorY >= m_nHeight)
		{
			m_nCursorY = 0;
		}
		DisplayGrid();
	}

	// Create patterns
	if (pInput->IsKeyPressed(Key::R))
	{
		ResetGrid();
	}
	if (pInput->IsKeyPressed(Key::G))
	{
		CreateGlider(m_nCursorX, m_nCursorY);
	}
	if (pInput->IsKeyPressed(Key::B))
	{
		CreateBeacon(m_nCursorX, m_nCursorY);
	}
	if (pInput->IsKeyPressed(Key::A))
	{
		RandomBlock(m_nCursorX, m_nCursorY, 5, 5);
	}

}

void Game::UpdateGrid()
{
	// Calculate what the next generation will look like
	for (int nY = 0; nY < m_nHeight; nY++)
	{
		for (int nX = 0; nX < m_nWidth; nX++)
		{
			int nNeighbors = GetNumberOfNeighbors(nX, nY);
			if (m_bGrid[nX][nY] == true)
			{
				if (nNeighbors < 2)
				{
					PlaceLife(nX, nY, false, m_bBufferGrid);
				}
				else if (nNeighbors < 4)
				{
					PlaceLife(nX, nY, true, m_bBufferGrid);
				}
				else if (nNeighbors > 3)
				{
					PlaceLife(nX, nY, false, m_bBufferGrid);
				}
			}
			else
			{
				if (nNeighbors == 3)
				{
					PlaceLife(nX, nY, true, m_bBufferGrid);
				}
			}
		}
	}

	// Copy the data into the main grid
	for (int nY = 0; nY < m_nHeight; nY++)
	{
		for (int nX = 0; nX < m_nWidth; nX++)
		{
			m_bGrid[nX][nY] = m_bBufferGrid[nX][nY];
		}
	}

}

void Game::DisplayGrid()
{
	system("cls");

	// Header info
	std::cout << "         Conway's Game of Life\n";
	std::cout << "  Use the arrow keys to move the cursor\n";
	std::cout << "  Place different patterns at the cursor\n";
	std::cout << "          R to reset the grid\n";
	std::cout << "          G to make a glider\n";
	std::cout << "          B to make a beacon\n";
	std::cout << "      A to make a random 5x5 pattern\n";
	std::cout << "         Current genaration: " << m_nGeneration << "\n";

	// Top part of border
	std::cout << "    +";
	for (int nX = 0; nX < m_nWidth; ++nX)
	{
		std::cout << "-";
	}
	std::cout << "+\n";

	// Grid information
	for (int nY = 0; nY < m_nHeight; ++nY)
	{
		std::cout << "    |";
		for (int nX = 0; nX < m_nWidth; ++nX)
		{
			if (nX == m_nCursorX && nY == m_nCursorY)
			{
				std::cout << "X";
			}
			else if (m_bGrid[nX][nY] == true)
			{
				std::cout << "O";
			}
			else
			{
				std::cout << ".";
			}
		}
		std::cout << "|\n";
	}

	// Bottom part of border
	std::cout << "    +";
	for (int nX = 0; nX < m_nWidth; ++nX)
	{
		std::cout << "-";
	}
	std::cout << "+";

}

void Game::ResetGrid()
{
	m_nGeneration = 0;
	m_bSimulating = false;

	for (int nX = 0; nX < m_nWidth; ++nX)
	{
		for (int nY = 0; nY < m_nHeight; ++nY)
		{
			m_bGrid[nX][nY] = false;
			m_bBufferGrid[nX][nY] = false;
		}
	}

}

int Game::GetNumberOfNeighbors(int nX, int nY)
{
	int nNeighbors = 0;
	for (int nDX = -1; nDX <= 1; nDX++)
	{
		for (int nDY = -1; nDY <= 1; nDY++)
		{
			int newX = nX + nDX;
			int newY = nY + nDY;

			if (newX >= 0 && newX < m_nWidth && newY >= 0 && newY < m_nHeight &&
				!(nDX == 0 && nDY == 0) &&
				m_bGrid[newX][newY] == true)
			{
				nNeighbors++;
			}
		}
	}

	return nNeighbors;

}

void Game::PlaceLife(int nX, int nY, bool bValue, bool ** pGrid)
{
	m_bSimulating = true;
	if (nX >= 0 && nX < m_nWidth && nY >= 0 && nY < m_nHeight)
	{
		pGrid[nX][nY] = bValue;
	}

}

void Game::RandomBlock(int nX, int nY, int nWidth, int nHeight)
{
	for (int nXX = 0; nXX < nWidth; ++nXX)
	{
		for (int nYY = 0; nYY < nHeight; ++nYY)
		{
			if (rand() % 2 == 0)
			{
				PlaceLife(nX + nXX, nY + nYY, true, m_bGrid);
			}
			else
			{
				PlaceLife(nX + nXX, nY + nYY, false, m_bGrid);
			}
		}
	}

}

void Game::CreateGlider(int nX, int nY)
{
	PlaceLife(nX + 1, nY + 0, true, m_bGrid);
	PlaceLife(nX + 2, nY + 1, true, m_bGrid);
	PlaceLife(nX + 0, nY + 2, true, m_bGrid);
	PlaceLife(nX + 1, nY + 2, true, m_bGrid);
	PlaceLife(nX + 2, nY + 2, true, m_bGrid);

}

void Game::CreateBeacon(int nX, int nY)
{
	PlaceLife(nX + 0, nY + 0, true, m_bGrid);
	PlaceLife(nX + 1, nY + 0, true, m_bGrid);
	PlaceLife(nX + 0, nY + 1, true, m_bGrid);
	PlaceLife(nX + 3, nY + 2, true, m_bGrid);
	PlaceLife(nX + 2, nY + 3, true, m_bGrid);
	PlaceLife(nX + 3, nY + 3, true, m_bGrid);

}