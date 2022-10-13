#ifndef  GAME_OBJECT_H
#define GAME_OBJECT_H
#include "SFML/Graphics.hpp"

/*Base Gameobject class, contains common variables and
	basic game object functions */
class GameObject
{
private:
	/*****************************************
	* MEMBER VARIABLES
	*****************************************/
	sf::Vector2f			_position;			//Stores current position
	sf::Vector2f			_direction;			//Stores current direction
	float					_speed;				//Stores current speed

protected:
	enum class				AXIS {X, Y, XY};	//Enum of axis to reverse

	/*Gets Current Speed */
	float			GetSpeed();	

	/*Gets Current Positions */
	sf::Vector2f	GetPosition();	

	/*Gets Current Direction */
	sf::Vector2f	GetDirection();	

	/*Gets current Velocity(direction * speed)*/
	sf::Vector2f	GetVelocity();	

	/*Will reverse the current direction on x, y, or both axis
	AXIS::X for x, AXIS::Y for y, AXIS::XY for both */
	void			ReverseDirection(AXIS axis);

public:
	/*****************************************
	* CONSTRUCTORS
	*****************************************/

	/*Basic Constructor*/
	GameObject();

	/*Constructor with at least position passed*/
	GameObject(	sf::Vector2f position,
				sf::Vector2f direction = sf::Vector2f(0, 0),
				float speed = 0 );

	/*****************************************
	* SETTERS
	*****************************************/

	/*Sets the position*/
	void	SetPosition(sf::Vector2f position);

	/*Sets the direction*/
	void	SetDirection(sf::Vector2f direction);

	/*Sets the speed*/
	void	SetSpeed(float speed);				
};

#endif // ! GAME_OBJECT_H