#include "Player.h"

Player::Player(sf::Vector2f position, float speed, sf::Vector2f size,
	sf::Color fill_color) : GameObject(position, sf::Vector2f(0, 0), speed)
{
	/*Initialize player variabls*/
	_size = size;
	_sprite.setSize(_size);
	_sprite.setFillColor(fill_color);

	/* Offset used to center player at creation */
	sf::Vector2f offset_position = this->GetPosition();
	offset_position.x -= _size.x / 2;
	_sprite.setPosition(offset_position);
	
}

void Player::Draw(sf::RenderWindow* render_window)
{
	render_window->draw(_sprite);
}

void Player::ProcessControls(sf::Event* event)
{
	/*Checks for a key pressed event*/
	if (event->type == sf::Event::KeyPressed)
	{
		/*Switch on Key that was pressed, if Left or A set direction to left, if
			D or Right Set to right. When Setting the direction we first make sure it
			hasn't already been set avoiding a call to set direction in preference of a
			check of get direction calling every update */
		switch (event->key.code)
		{
		case sf::Keyboard::Left:
		case sf::Keyboard::A:
			if (this->GetDirection().x != -1)
			{
				this->SetDirection(sf::Vector2f(-1, 0));
			}
			break;
		case sf::Keyboard::Right:
		case sf::Keyboard::D:
			if (this->GetDirection().x != 1)
			{
				this->SetDirection(sf::Vector2f(1, 0));
			}
			break;
		default:
			break;
		}
	}
	/* If Key was released */
	else if (event->type == sf::Event::KeyReleased)
	{
		/*Switch on Key that was released, if Left AND the controls for moving 
			right are not pressed then set direction to 0 to stop moving, vice versa for
			the right controls */
		switch (event->key.code)
		{
		case sf::Keyboard::Left:
		case sf::Keyboard::A:
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D) &&
				!sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				this->SetDirection(sf::Vector2f(0, 0));
			}
			else
			{
				this->SetDirection(sf::Vector2f(1, 0));
			}
			break;
		case sf::Keyboard::Right:
		case sf::Keyboard::D:
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
				!sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				this->SetDirection(sf::Vector2f(0, 0));
			}
			else
			{
				this->SetDirection(sf::Vector2f(-1, 0));
			}
			break;
		default:
			break;
		}
	}
	
}

void Player::Move(sf::Vector2f window_bounds, bool constrain)
{
	/* Variable used to calculate a new position set to current position by default */
	sf::Vector2f new_position = this->GetPosition();

	/* Start by setting the new position to the current plus the velocity*/
	new_position += this->GetVelocity();

	/* Get our minimum and maximum x positions */
	float minimum_x_position = window_bounds.x;
	float maximum_x_position = window_bounds.y;

	/* If the player is not constrained then moving far right will loop player
		onto the left of the screen and going left will loop to the right */
	if (!constrain)
	{
		if (new_position.x + _sprite.getGlobalBounds().width < minimum_x_position)
			new_position.x = maximum_x_position;

		else if (new_position.x - _sprite.getGlobalBounds().width > maximum_x_position)
			new_position.x = minimum_x_position;
	}
	/* If player is constrained stop player from moving past edge of window */
	else
	{
		if (new_position.x < minimum_x_position)
			new_position.x = minimum_x_position;

		else if (new_position.x > maximum_x_position)
			new_position.x = maximum_x_position;
	}

	/* Set position to the new position */
	this->SetPosition(new_position);
}

void Player::Update(sf::Vector2f window_bounds, bool constrain)
{
	/* Moves the player as needed and updates the sprite position appropriately */
	this->Move(window_bounds, constrain);
	this->_sprite.setPosition(this->GetPosition());
}

/* Function gets the current player position */

sf::Vector2f Player::GetCurrentPosition() 
{ 
	return this->GetPosition(); 
}

sf::FloatRect Player::GetBounds() 
{ 
	/* Returns the bounding box of the player object */
	return _sprite.getGlobalBounds(); 
}