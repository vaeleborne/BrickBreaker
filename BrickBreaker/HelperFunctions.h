#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H
#include "SFML/Graphics.hpp"
#include <math.h>

namespace helpers
{
	/* Used to see if */
	extern bool IS_SEEDED;

	/* Gets a random number between min and max inclusively */
	int RandomNumber(int min, int max);

	/* enum class for our game states*/
	enum class GAMESTATE { TITLE, PLAY, GAMEOVER, EXIT };
}

#endif