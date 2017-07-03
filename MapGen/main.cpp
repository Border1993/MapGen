#include "Building.h"
#include "MapGenerator.h"

#include <stdio.h>
#include <Windows.h>

int main()
{
	Building field;
	field.sizeX = 3;
	field.sizeY = 2;
	field.id = 1000;
	field.count = 2;
	field.minDistance = 7.0f;
	field.connections0.push_back(ConnectionPoint(-1, 0));
	field.connections90.push_back(ConnectionPoint(1, -1));
	field.connections180.push_back(ConnectionPoint(3, 1));
	field.connections270.push_back(ConnectionPoint(0, 3));

	Building hotel;
	hotel.sizeX = 3;
	hotel.sizeY = 2;
	hotel.id = 1001;
	hotel.count = 2;
	hotel.minDistance = 8.0f;
	hotel.connections0.push_back(ConnectionPoint(-1, 0));
	hotel.connections0.push_back(ConnectionPoint(0, -1));
	hotel.connections90.push_back(ConnectionPoint(1, -1));
	hotel.connections90.push_back(ConnectionPoint(2, 0));
	hotel.connections180.push_back(ConnectionPoint(3, 1));
	hotel.connections180.push_back(ConnectionPoint(2, 2));
	hotel.connections270.push_back(ConnectionPoint(0, 3));
	hotel.connections270.push_back(ConnectionPoint(-1, 2));


	MapGenerator mapGen;
	mapGen.buildingsInfo.push_back(field);
	mapGen.buildingsInfo.push_back(hotel);
	mapGen.Generate();

	system("PAUSE");

	return 0;
}