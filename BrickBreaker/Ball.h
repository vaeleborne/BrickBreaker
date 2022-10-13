#ifndef BALL_H
#define BALL_H
#include "GameObject.h"
#include "HelperFunctions.h"
#include "Brick.h"
#include "Player.h"
#include <iostream>

/*Player Game Object, contains information for a player object, as well as
	functions for drawing, updating, moving, and deleting said object */
class Ball : GameObject {
private:
	float				_radius;	//Member variable determining size of ball
	sf::CircleShape		_sprite;	//Member variable to hold our 'sprite'
	bool				_render;	//Determines if object should render

	/* Function will calculate movement of the object.
	   With passed constraints */
	void Move(sf::Vector2f min_values = sf::Vector2f(0, 0),
		sf::Vector2f max_values = sf::Vector2f(0, 0));

	/*Function will flag this game object to be destroyed */
	void SetToDestroy();
	
public:

	/* CONSTRUCTORS, has a default constructor, and one that requires at least a position */
	Ball();
	Ball(	sf::Vector2f position, sf::Vector2f direction = sf::Vector2f(1, 1), 
			float speed = 10, float radius = 5, 
			sf::Color fill_color = sf::Color::White);
		
	
	/* The update function for this object. Should do primary
		things needed every game update call, such as calculating
		the movement of the ball */
	void Update(sf::Vector2f min_values = sf::Vector2f(0, 0),
		sf::Vector2f max_values = sf::Vector2f(0, 0));

	/* Function to draw the ball to the render window */
	void Draw(sf::RenderWindow* render_window);	

	/*Gets the bounding box of the ball*/
	sf::FloatRect GetBounds();

	/*Gets the current positions of the ball*/
	sf::Vector2f GetCurrentPosition();
	
	/*Gets the current direction of the ball */
	sf::Vector2f GetCurrentDirection();

	/*Reverse the x direction of the ball */
	void ReverseDirectionX();

	/*Reverse the y direction of the ball*/
	void ReverseDirectionY();

	/* Simple function to test if this object should get destroyed */
	bool ShouldDestroy();

	/*Used on ball launch to add some randomness to the start */
	void SetRandomUpwardDirection();
};

#endif // !BALL_H

