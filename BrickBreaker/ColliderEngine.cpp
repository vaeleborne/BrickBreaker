#include "ColliderEngine.h"

bool ColliderEngine::CheckForCollision(Ball* ball, Player* player)
{
	/*Our return variable set to false for no collision by default */
	bool			collided = false;

	/* Obtain the bounding box for each passed in object */
	sf::FloatRect	ball_bounds = ball->GetBounds();
	sf::FloatRect	player_bounds = player->GetBounds();


	/*Adding a bit of a margin of error based off of the ball's
	  bounding box size to help prevent ball getting
	  stuck in player, shouldn't be a noticeable difference visually  */
	player_bounds.height -= ball_bounds.height;

	/* Checks if our bounds are intersecting */
	if (ball_bounds.intersects(player_bounds))
	{
		/*Intersecting so we flag our return as true*/
		collided = true;

		/*We are getting positions here to test if the ball hit the left or right 
			of the player, if it hit the left and was going right we also 
			reverse the x direction and vice versa if the ball hits the right but was
			going left */
		sf::Vector2f ball_position = ball->GetCurrentPosition();
		sf::Vector2f player_position = player->GetCurrentPosition();
		sf::Vector2f player_size = sf::Vector2f(player_bounds.width,
			player_bounds.height);

		/* Ball hit right side, see if it was going left, if so make it go right */
		if (ball_position.x > player_position.x + player_size.x / 2)
		{
			if (ball->GetCurrentDirection().x < 0)
				ball->ReverseDirectionX();
		}
		/* Ball hit left side, see if it was going right, if so make it go left */
		else if (ball_position.x <= player_position.x + player_size.x / 2)
		{
			if (ball->GetCurrentDirection().x > 0)
				ball->ReverseDirectionX();
		}
		ball->ReverseDirectionY();
	}

	return collided;
}

bool ColliderEngine::CheckForCollision(Ball* ball, Brick* brick)
{

	//Return Variable Initialized To False
	bool			collided = false;

	//Obtaining each objects bounding rectangle
	sf::FloatRect	ball_bounds = ball->GetBounds();
	sf::FloatRect	brick_bounds = brick->GetBounds();

	/*See if the ball's bounding rectangle is intersecting
		the brick object's*/
	if (ball_bounds.intersects(brick_bounds))
	{
		//COLLISION WAS DETECTED!

		//Flagging the brick object to be destroyed
		brick->SetToDestroy();

		/* Setting our return variable to true since there was
		 a collision detected. */
		collided = true;

		/* Creating some local variables to hold each objects
		 * current position.
		 *	IMPORTANT: Position is based of of the shapes ORIGIN
		 *		For 'this' circleshape(ball) the origin is in the
		 *		middle of the object. HOWEVER, for the brick the
		 *		origin is in the Upper Left, same with all
		 *		SFML rectangles by default
		 */

		sf::Vector2f ball_position = ball->GetCurrentPosition();
		sf::Vector2f brick_position = brick->GetCurrentPosition();

		sf::Vector2f ball_size = sf::Vector2f(ball_bounds.width, ball_bounds.height);

		/* Create a variable to hold the dimensions of the brick */
		sf::Vector2f brick_size = sf::Vector2f(brick_bounds.width,
			brick_bounds.height);

		/* Create a variable to hold half of the total dimensions of
		 the brick, will be useful for testing where the ball
		 collided */
		sf::Vector2f half_brick_size = sf::Vector2f(
			brick_bounds.width / 2,
			brick_bounds.height / 2);

		/* Check to see if the ball's horizontal position is greater
		* than(or equal) to the center point of the brick, if so
		* we will consider the ball to be on the right side of the
		* brick, remember brick position is the upper left
		* corner of the brick so we add the half width
		*/
		if (ball_position.x >= brick_position.x + half_brick_size.x)
		{
			//COLLISION IS ON THE RIGHT SIDE OF THE BRICK

			/* We are going to see if the ball's horizontal
			*  position (center of ball) is greater than or
			*  equal to the far right horizontal position of the
			*  brick (pos + width) MINUS the size of our balls
			*  bounds as the offset. If this is true then
			*  we know the ball collided on the right edge of the
			*  brick (or possibly a right corner but we will count
			*  that as right side.
			*/
			if (ball_position.x > brick_position.x + brick_size.x - ball_size.x)
			{
				/* Ball hit the right edge of a brick!
				 * We will make it reverse horizontal direction */
				ball->ReverseDirectionX();
			}
			else
			{
				/*	Collision was on the right half of the brick,
				*	However, it was not on the right edge, so
				*	We can infer it either hit the top right OR
				*	The bottom right, regardless we want to
				*	Reverse the vertical direction of the ball */
				ball->ReverseDirectionY();
			}
		}

		/* Check to see if the ball's horizontal position is less
		* than the position of the brick(upper left corner of brick.
		* If so we will consider the ball to be on the left side
		* of the brick
		*/
		else if (ball_position.x < brick_position.x + half_brick_size.x)
		{
			//COLLISION IS ON THE LEFT SIDE OF THE BRICK

			/* We are going to see if the ball's horizontal
			*  position (center of ball) is less than or
			*  equal to the left horizontal position of the
			*  brick. If this is true then
			*  we know the ball collided on the left edge of the
			*  brick (or possibly a left corner but we will count
			*  that as left side.
			*/
			if (ball_position.x <= brick_position.x)
			{
				ball->ReverseDirectionX();
			}

			/*	Collision was on the left half of the brick,
			*	However, it was not on the left edge, so
			*	We can infer it either hit the top left OR
			*	The bottom left, regardless we want to
			*	Reverse the vertical direction of the ball */
			else
			{
				ball->ReverseDirectionY();
			}
		}

	}

	//Return the value of collided 
	return collided;
}