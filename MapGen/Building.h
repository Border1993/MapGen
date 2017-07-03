#pragma once
#include <stdio.h>
#include <string>
#include <vector>

struct ConnectionPoint
{
	ConnectionPoint()
	{
		x = 0;
		y = 0;
	}

	ConnectionPoint(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	int x;
	int y;
};

class Building
{
public:
	int id;
	int count;
	int sizeX;
	int sizeY;
	float minDistance;
	
	std::vector<ConnectionPoint> connections0;
	std::vector<ConnectionPoint> connections90;
	std::vector<ConnectionPoint> connections180;
	std::vector<ConnectionPoint> connections270;

	bool NotTooClose(int x, int y);

	std::vector<float> positionX;
	std::vector<float> positionY;
	std::vector<float> rotation;
};