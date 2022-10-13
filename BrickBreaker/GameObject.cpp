#include "GameObject.h"

#pragma region PROTECTED
/*Simple Getter Functions*/

float GameObject::GetSpeed()				{ return _speed; }
sf::Vector2f GameObject::GetPosition()		{ return _position; }
sf::Vector2f GameObject::GetDirection()		{ return _direction; }


sf::Vector2f GameObject::GetVelocity()
{
	/*Creating our return variable that will contain the velocity*/
	sf::Vector2f velocity(0, 0);

	/*	Velocity = Direction times Speed, doing the math and
	 *	setting the return variable appropriately */

	velocity.x = this->GetDirection().x * this->GetSpeed();
	velocity.y = this->GetDirection().y * this->GetSpeed();

	/*Return the calculated velocity of the game object*/
	return velocity;
}

void GameObject::ReverseDirection(AXIS axis)
{
	/*Creating a variable used to set the new reversed direction*/
	sf::Vector2f new_direction = this->GetDirection();

	/*Check which axis needs reversed then reverse*/
	if(axis == AXIS::X || axis == AXIS::XY)
		new_direction.x *= -1;

	if(axis == AXIS::Y || axis == AXIS::XY)
		new_direction.y *= -1;

	/*Set the current direction to the newly reversed direction*/
	this->SetDirection(new_direction);
}
#pragma endregion


#pragma region PUBLIC
GameObject::GameObject() : _position(0, 0), 
						_direction(0,0), _speed(0) {}

GameObject::GameObject( sf::Vector2f position, 
						sf::Vector2f direction,
						float speed ) 
				:	_position(position), _direction(direction),
					_speed(speed) {}

/*Basic Setter Functions */
void GameObject::SetDirection(sf::Vector2f direction)
{
	_direction = direction;
}

void GameObject::SetSpeed(float speed)
{
	_speed = speed;
}

void GameObject::SetPosition(sf::Vector2f position)
{
	_position = position;
}

#pragma endregion 
