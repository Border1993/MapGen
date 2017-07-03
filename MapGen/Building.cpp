#include "Building.h"

bool Building::NotTooClose(int x, int y)
{
	for (int i = 0; i < this->positionX.size(); i++)
	{
		float dist = sqrtf((float)pow((x - positionX[i]), 2) + (float)pow(y - positionY[i], 2));
		if (dist < this->minDistance) return false;
	}

	return true;
}