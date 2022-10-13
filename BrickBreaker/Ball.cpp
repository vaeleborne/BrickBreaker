#include "Ball.h"

#pragma region Private
void Ball::Move( sf::Vector2f min_values, sf::Vector2f max_values)
{
	/*If the ball should be rendered*/
	if (_render)
	{
		/*Calculate the new position given restraints*/
		sf::Vector2f new_position = this->GetPosition();

		new_position += this->GetVelocity();

		/*Ball hit left edge of window reverse x direction*/
		if (new_position.x < min_values.x)
		{
			new_position.x = min_values.x;
			this->ReverseDirection(AXIS::X);
		}
		/*Ball hit right edge of window reverse x direction*/
		else if (new_position.x > max_values.x)
		{
			new_position.x = max_values.x;
			this->ReverseDirection(AXIS::X);
		}
		/*Ball hit top edge of window reverse y direction*/
		if (new_position.y < min_values.y)
		{
			new_position.y = min_values.y;
			this->ReverseDirection(AXIS::Y);
		}
		/*Ball went through bottom of window, set to destroy ball, this will trigger 
			the game over state */
		else if (new_position.y > max_values.y)
		{
			this->SetToDestroy();
		}

		/* Set the balls position to the calculated position */
		this->SetPosition(new_position);
	}
}

void Ball::SetToDestroy()
{
	_render = false; 
}
#pragma endregion


#pragma region Public
Ball::Ball() : GameObject::GameObject() {
	_radius = 5;
	_render = true;
	_sprite.setRadius(_radius);
	_sprite.setFillColor(sf::Color::White);
	_sprite.setPosition(this->GetPosition());

}

Ball::Ball(sf::Vector2f position, sf::Vector2f direction,
	float speed, float radius, sf::Color fill_color) :
	GameObject(position, direction, speed), _radius(radius)
{	
	_render = true;
	_sprite.setRadius(_radius);
	_sprite.setFillColor(fill_color);
	_sprite.setPosition(this->GetPosition());
}

void Ball::Update(sf::Vector2f min_values, sf::Vector2f max_values)
{
	if (_render)
	{
		this->Move(min_values, max_values);
		this->_sprite.setPosition(this->GetPosition());
	}
}

void Ball::Draw(sf::RenderWindow* render_window)
{
	if (_render)
	{
		//Draws our sprite to a render window
		render_window->draw(_sprite);
	}
}

void Ball::ReverseDirectionX() { this->ReverseDirection(AXIS::X); }

void Ball::ReverseDirectionY() { this->ReverseDirection(AXIS::Y); }

bool Ball::ShouldDestroy()
{
	return !_render;
}



/*Getter functions */
sf::FloatRect Ball::GetBounds() 
{ 
	return _sprite.getGlobalBounds();
}

sf::Vector2f Ball::GetCurrentPosition()
{ 
	return this->GetPosition();
}

sf::Vector2f Ball::GetCurrentDirection() 
{ 
	return this->GetDirection();
}

/*Setter functions */
void Ball::SetRandomUpwardDirection() {
	/* Variable to see if we want to bounce left or right */
	float left_or_right = helpers::RandomNumber(0, 1);

	/* Variables for direction */
	float dir_x;
	float dir_y;

	if (left_or_right == 0)
	{
		dir_x = -helpers::RandomNumber(30, 80) / 100.f;
	}
	else
	{
		dir_x = helpers::RandomNumber(30, 80) / 100.f;
	}

	dir_y = -helpers::RandomNumber(20, 50) / 100.f;

	SetDirection(sf::Vector2f(dir_x, dir_y));
}

#pragma endregion




