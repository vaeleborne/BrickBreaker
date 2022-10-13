#ifndef  BRICK_H
#define	 BRICK_H
#include "GameObject.h"

/*Brick Game Object, contains information for a brick object, as well as
	functions for drawing, destroying said object */
class Brick : GameObject
{
private:
	sf::Vector2f			_size;			//Member variable holding size of brick			
	sf::RectangleShape		_sprite;		//The drawable variable for the brick
	bool					_render;		//Determines if the brick should be drawn
	unsigned int			_score_value;	//The value for destroying this brick
	

public:

	/* Simple function to obtain the bounding box for the sprite */
	sf::FloatRect GetBounds();
	
	/* Constructor, MUST at least give a position */
	Brick(sf::Vector2f position, sf::Vector2f size = sf::Vector2f(50, 10), 
		unsigned int score = 25, sf::Color fill_color = sf::Color::Red);

	/*Function to check if the instance should be destroyed */
	bool ShouldDestroy();

	/* Function to flag this object for destruction */
	void SetToDestroy();

	void SetScore(unsigned int score);

	/* Function to set the texture of the sprite */
	void SetTexture(sf::Texture* texture);

	/* Function to get the score value of this brick */
	unsigned int GetScore();

	/* Function to draw brick to a render window */
	void Draw(sf::RenderWindow* render_window);

	//Need for the ball objects collision detection as GetPosition is protected
	sf::Vector2f GetCurrentPosition();
};

#endif // ! BRICK_H