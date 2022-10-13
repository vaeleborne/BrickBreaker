#include "HelperFunctions.h"
bool helpers::IS_SEEDED = false;

int helpers::RandomNumber(int min, int max)
{
	//Make sure rand has been seeded
	if (!IS_SEEDED)
	{
		std::srand(std::time(nullptr));
		IS_SEEDED = true;
	}
	//Return the random number
	return (rand() % (max - min + 1) + min);
}

