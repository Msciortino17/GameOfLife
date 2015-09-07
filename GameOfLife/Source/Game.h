/****************************************************
* Class:			Game.h
* Author:			Michael Sciortino
* Date Created:		09/06/15
* Last Modified:	09/07/15
* Purpose:			Central hub for all things related to gameplay
*****************************************************/

#pragma once

#include "XTime.h"

class Game
{
private:

	bool m_bRunning;
	int m_nWidth;
	int m_nHeight;
	bool ** m_bGrid;
	bool ** m_bBufferGrid;
	int m_nCursorX;
	int m_nCursorY;
	int m_nGeneration;
	bool m_bSimulating;

	// Time related variables
	XTime m_xTime;
	float m_fDeltaTime;
	float m_fTimer;
	float m_fUpdateRate;

	// General helper functions
	void UpdateMenu();
	void UpdateGrid();
	void DisplayGrid();
	int GetNumberOfNeighbors(int nX, int nY);

	// Functions for placing new life and making patterns
	void ResetGrid();
	void PlaceLife(int nX, int nY, bool bValue, bool ** pGrid);
	void RandomBlock(int nX, int nY, int nWidth, int nHeight);
	void CreateGlider(int nX, int nY);
	void CreateBeacon(int nX, int nY);


public:

	/****************************************************
	* Function: Initialize
	* Parameters:
	*	In: 
	*		nWidth: Width of the map
	*		nHeight: Height of the map
	*	Out: void
	* Return: void
	* Purpose: Starting up everything for the game
	*****************************************************/
	void Initialize(int nWidth, int nHeight);

	/****************************************************
	* Function: Terminate
	* Parameters:
	*	In: void
	*	Out: void
	* Return: void
	* Purpose: Shutdown and cleaning up memory
	*****************************************************/
	void Terminate();

	/****************************************************
	* Function: Update
	* Parameters:
	*	In: void
	*	Out: void
	* Return: void
	* Purpose: Main run loop
	*****************************************************/
	void Update();

	/****************
	* Accessors
	*****************/
	bool GetRunning() const { return m_bRunning; }

	/****************
	* Mutators
	*****************/
	void SetRunning(bool _running) { m_bRunning = _running; }

};