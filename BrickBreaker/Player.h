#ifndef PLAYER_H
#define PLAYER_H
#include "GameObject.h"
#include "SFML/Graphics.hpp"
#include <iostream>

/*Player Game Object, contains information for a player object, as well as
	functions for drawing, updating, and moving said object */
class Player : GameObject
{
private:
	sf::Vector2f			_size;		//Holds the size of the player
	sf::RectangleShape		_sprite;	//Member variable to hold our 'sprite' not SFML SPRITE type

	/*Function to update position, player object can ONLY
	move left or right so we just need to constrain on x axis*/
	void Move(sf::Vector2f window_bounds, bool constrain = false );

public:
	/* Function to get the bounding box for the player */
	sf::FloatRect GetBounds();

	/* Function to draw the player onto a render window */
	void Draw(sf::RenderWindow* render_window);

	/*Constructor*/
	Player(sf::Vector2f position, float speed = 15,
		sf::Vector2f size = sf::Vector2f(70, 20),
		sf::Color fill_color = sf::Color::White);

	/* Function to process controls for movement purposes */
	void ProcessControls(sf::Event* event);

	/* Function updates the player object, processes movement */
	void Update(sf::Vector2f window_bounds = sf::Vector2f(0, 0), bool constrain = false );

	/* Function gets the current player position */
	sf::Vector2f GetCurrentPosition();
};

#endif // !PLAYER_H

