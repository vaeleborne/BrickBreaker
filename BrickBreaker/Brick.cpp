#include "Brick.h"


/* Simple function to obtain the bounding box for the sprite */

sf::FloatRect Brick::GetBounds()
{
	return _sprite.getGlobalBounds();
}

Brick::Brick(sf::Vector2f position, sf::Vector2f size,
	unsigned int score ,sf::Color fill_color) : 
	GameObject(position), _size(size), _render(true),
	_score_value(score)
{
	_sprite.setSize(_size);
	_sprite.setFillColor(fill_color);
	_sprite.setPosition(GetPosition());
	
}

bool Brick::ShouldDestroy()
{
	//If render is true then we should NOT destroy so 
	//We simply return the negative of render as if it is
	//False we SHOULD destroy
	return !_render;
}

void Brick::Draw(sf::RenderWindow* render_window)
{
	if (_render)
	{
		render_window->draw(_sprite);
	}
}

/*Setter Functions */

void Brick::SetToDestroy() 
{
	_render = false; 
}

void Brick::SetScore(unsigned int score)
{ 
	_score_value = score; 
}

void Brick::SetTexture(sf::Texture* texture)
{
	_sprite.setTexture(texture);
}

/*Getter Functions */

unsigned int Brick::GetScore() 
{ 
	return _score_value; 
}

sf::Vector2f Brick::GetCurrentPosition()
{
	return this->GetPosition();
}





