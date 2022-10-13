#ifndef COLLIDER_ENGINE
#define COLLIDER_ENGINE
#include "Brick.h"
#include "Player.h"
#include "Ball.h"
class ColliderEngine
{
public:
	/* Function will check for a collision of a ball and a player. If
		the ball is colliding with a player it will bounce off of it*/
	static bool CheckForCollision(Ball* ball, Player* player);

	/* Function will check for a collision of a ball and a brick. If
		the ball is colliding with a brick it will bounce off of it and
		flag the brick to be destroyed (which will trigger the score updating */
	static bool CheckForCollision(Ball* ball, Brick* brick);

};
#endif // !COLLIDER_ENGINE
