/***************************************************************************************
*		TITLE: Brick Breaker
*		OVERVIEW: A Breakout / Brick Breaker Clone Done With SFML and C++
* 
*		AUTHOR: Dylan Morgan
* 
*		LAST EDITED: 10/11/2022
* 
*		NOTES FOR NEXT:
*			Continue working on Ball Class, refine Move function, possibly
*			Abstract to outer namespace of helper functions for constraint
*			
*			Refine Draw function in objects singleton, make 
*				rename to Update, have it call all objects 
*				updates, then draws
*				Add Brick collision to ball class
* 
*			Add random angle to ball bounce (between angle alpha 5 and 45)
*				Rotate vector ( [cos alpha, sin alpha] )
* 
*****************************************************************************************/

#include "Ball.h"
#include "Player.h"
#include "GameController.h"
#include "HelperFunctions.h"
#include <string>
#include <vector>
#include <iostream>
#include "SFML/Audio.hpp"

//Create instance of our singleton class that contains all game object
//References
 GameController* GAME_CONTROLLER = GameController::GetInstance();


int main()
{
	/* CONSTANT GLOBAL VARIABLES, USED FOR RENDER WINDOW SETUP */
	const unsigned int		WINDOW_WIDTH = 1200;
	const unsigned int		WINDOW_HEIGHT = 556;
	const unsigned int		FRAME_RATE_LIMIT = 60;
	const std::string		WINDOW_TITLE = "My Brick Breaker!";

	/* Creating our render window*/
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), 
		WINDOW_TITLE);

	/* Setting the frame rate limit */
	window.setFramerateLimit(FRAME_RATE_LIMIT);

	/* Attempt to initialize our game */
	try {
		GAME_CONTROLLER->InitializeGame(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
	catch (int) {
		/* If during initialization an error is thrown, close
			the render window, and release our game controller
			object so everything deletes properly */
		window.close();
		GAME_CONTROLLER->ReleaseInstance();
		return -1;
	}

	GAME_CONTROLLER->RunGameControl(&window, WINDOW_WIDTH, WINDOW_HEIGHT);

	GAME_CONTROLLER->ReleaseInstance();
	return 0;
}

\

