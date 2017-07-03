// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGenerator.h"

MapGenerator::MapGenerator()
{
	this->sizeX = 21+20;
	this->sizeY = 21+10;
	this->minimumDistance = 4.0f;
	this->blocks = nullptr;
	this->visible = nullptr;
	this->canDelete = nullptr;
	this->buildings = nullptr;

	this->numGroundFloors = 3;
	this->numHigherFloors = 1;

	

	points = 12;



}

void MapGenerator::BuildArrays()
{
	if (this->blocks != nullptr) delete[] this->blocks;
	if (this->visible != nullptr) delete[] this->visible;
	if (this->canDelete != nullptr) delete[] this->canDelete;
	if (this->buildings != nullptr) delete[] this->buildings;


	this->blocks = new int[sizeX * sizeY];
	this->visible = new bool[sizeX * sizeY];
	this->canDelete = new bool[sizeX * sizeY];
	this->buildings = new std::vector<int>[sizeX * sizeY];

	int size = sizeX * sizeY;

	for (int i = 0; i < size; i++)
	{
		blocks[i] = -1;
		visible[i] = true;
		canDelete[i] = true;
	}

	for (int i = 0; i < size; i++)
	{
		buildings[i].reserve(10);
	}
}

void MapGenerator::SetBlock(Pt point, int value)
{
	if (point.x < 0) return;
	if (point.x >= sizeX) return;
	if (point.y < 0) return;
	if (point.y >= sizeY) return;
	
	blocks[point.x + point.y*sizeX] = value;
}

void MapGenerator::GenerateSpecialBuildings()
{

	for (int i = 0; i < buildingsInfo.size(); i++)
	{
		for (int j = 0; j < buildingsInfo[i].count; j++)
		{
			int rotation = rand() % 4;
			int x = rand() % (sizeX - 2) + 1;
			int y = rand() % (sizeY - 2) + 1;

			if (!buildingsInfo[i].NotTooClose(x, y))
			{
				j--;
				continue;
			}
			
			if (rotation == 0 || rotation == 2)
			{
				if (TryReserve(x, y, buildingsInfo[i].sizeX, buildingsInfo[i].sizeY, buildingsInfo[i].id))
				{
					if (rotation == 0)
					{
						for (int k = 0; k < buildingsInfo[i].connections0.size(); k++)
						{
							Pt point = Pt(buildingsInfo[i].connections0[k].x + x, buildingsInfo[i].connections0[k].y + y);
							canDelete[point.x + point.y * sizeX] = false;
							PointsToConnect.push_back(point);
						}
						buildingsInfo[i].rotation.push_back(0);
						std::cout << "0\n";
					}
					else
					{
						for (int k = 0; k < buildingsInfo[i].connections180.size(); k++)
						{
							Pt point = Pt(buildingsInfo[i].connections180[k].x + x, buildingsInfo[i].connections180[k].y + y);
							canDelete[point.x + point.y * sizeX] = false;
							PointsToConnect.push_back(point);
						}
						buildingsInfo[i].rotation.push_back(180);
						std::cout << "180\n";
					}
					buildingsInfo[i].positionX.push_back(x);
					buildingsInfo[i].positionY.push_back(y);
					

				}
				else
				{
					j--;
					continue;
				}
			}
			else
			{ 
				if (TryReserve(x, y, buildingsInfo[i].sizeY, buildingsInfo[i].sizeX, buildingsInfo[i].id))
				{
					if (rotation == 1)
					{
						for (int k = 0; k < buildingsInfo[i].connections90.size(); k++)
						{
							Pt point = Pt(buildingsInfo[i].connections90[k].x + x, buildingsInfo[i].connections90[k].y + y);
							canDelete[point.x + point.y * sizeX] = false;
							PointsToConnect.push_back(point);
						}
						buildingsInfo[i].rotation.push_back(90);
						std::cout << "90\n";
					}
					else
					{
						for (int k = 0; k < buildingsInfo[i].connections270.size(); k++)
						{
							Pt point = Pt(buildingsInfo[i].connections270[k].x + x, buildingsInfo[i].connections270[k].y + y);
							canDelete[point.x + point.y * sizeX] = false;
							PointsToConnect.push_back(point);
						}
						buildingsInfo[i].rotation.push_back(270);
						std::cout << "270\n";
					}
					buildingsInfo[i].positionX.push_back(x);
					buildingsInfo[i].positionY.push_back(y);
				}
				else
				{
					j--;
					continue;
				}
			}
		}
	}

	

}

bool MapGenerator::TryReserve(int x, int y, int width, int height, int building)
{
	x--;
	y--;

	width += 2;
	height += 2;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (this->GetBlock(Pt(x + j, y + i)) != NOTHING) return false;
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			SetBlock(Pt(x + j, y + i), RESERVED_FOR_ROAD);
		}
	}

	x++;
	y++;

	width -= 2;
	height -= 2;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			SetBlock(Pt(x + j, y + i), building);
			visible[x + j + (y + i) * sizeX] = false;
		}
	}

	return true;
}

bool MapGenerator::CanConnectLeft(int x, int y)
{
	Pt point = Pt(x - 1, y);
	if (!InRange(point)) return false;
	else
	{
		int block = GetBlock(point);
		if (block > 0) return true;
		else return false;
	}
}

bool MapGenerator::CanConnectRight(int x, int y)
{
	Pt point = Pt(x + 1, y);
	if (!InRange(point)) return false;
	else
	{
		int block = GetBlock(point);
		if (block > 0) return true;
		else return false;
	}
}

bool MapGenerator::CanConnectUp(int x, int y)
{
	Pt point = Pt(x, y - 1);
	if (!InRange(point)) return false;
	else
	{
		int block = GetBlock(point);
		if (block > 0) return true;
		else return false;
	}
}

bool MapGenerator::CanConnectDown(int x, int y)
{
	Pt point = Pt(x, y + 1);
	if (!InRange(point)) return false;
	else
	{
		int block = GetBlock(point);
		if (block > 0) return true;
		else return false;
	}
}


bool MapGenerator::CanGoLeft(Pt current)
{
	current.x--;


	if (current.x < 0 || current.x >= sizeX) return false;
	if (current.y < 0 || current.y >= sizeY) return false;
	if (blocks[current.x + current.y * sizeX] > -1) return false;
	return true;
}

bool MapGenerator::CanGoRight(Pt current)
{
	current.x++;

	if (current.x < 0 || current.x >= sizeX) return false;
	if (current.y < 0 || current.y >= sizeY) return false;
	if (blocks[current.x + current.y * sizeX] > -1) return false;
	return true;
}

bool MapGenerator::CanGoDown(Pt current)
{
	current.y++;

	if (current.x < 0 || current.x >= sizeX) return false;
	if (current.y < 0 || current.y >= sizeY) return false;
	if (blocks[current.x + current.y * sizeX] > -1) return false;
	return true;
}

bool MapGenerator::CanGoUp(Pt current)
{
	current.y--;
	if (current.x < 0 || current.x >= sizeX) return false;
	if (current.y < 0 || current.y >= sizeY) return false;
	if (blocks[current.x + current.y * sizeX] > -1) return false;
	return true;
}

void MapGenerator::CleanUp()
{
	for (int i = 0; i < sizeX * sizeY; i++)
	{
		if (blocks[i] == NOTHING) blocks[i] = 0;
		if (blocks[i] == RESERVED_FOR_ROAD) blocks[i] = 0;

		if (blocks[i] == 0)
		{
			bool pass = false;
			Pt point;
			point.x = i % sizeX;
			point.y = i / sizeX;

			for (int j = -1; j <= 1; j++)
			{
				for (int k = -1; k <= 1; k++)
				{
					Pt nPoint = Pt(point.x + j, point.y + k);
					bool isInRange = InRange(nPoint);
					int block = 0;

					if (isInRange)
					{
						block = GetBlock(nPoint);
					}

					if (block != 0 && block != -1) pass = true;
				}
			}

			//I cast thee back into nothigness!
			if (!pass) blocks[i] = NOTHING;
		}

	}



}

void MapGenerator::DeleteUnneceseary()
{
	bool changed = false;

	do
	{
		changed = false;

		for (int y = 0; y < sizeY; y++)
		{
			for (int x = 0; x < sizeX; x++)
			{
				if (canDelete[x + y * sizeX] == false) continue;

				if (GetBlock(Pt(x, y)) == ROAD_IN_PROGRESS)
				{
					bool surrounding[3][3];
					for (int i = 0; i < 3; i++)
					{
						for (int j = 0; j < 3; j++)
						{
							if (GetBlock(Pt(x + (i - 1), y + (j - 1))) == ROAD_IN_PROGRESS) surrounding[i][j] = true;
							else surrounding[i][j] = false;
						}
					}

					for (int i = 0; i < 3; i++)
					{
						for (int j = 0; j < 3; j++)
						{
							if (GetBlock(Pt(x + (i - 1), y + (j - 1))) == ROAD_IN_PROGRESS) surrounding[i][j] = true;
							else surrounding[i][j] = false;
						}
					}

					if (CheckCurve(surrounding))
					{
						blocks[x + y * sizeX] = 0;
						changed = true;
					}
					else if (Check3way(surrounding))
					{
						blocks[x + y * sizeX] = 0;
						changed = true;
					}
				}
			}
		}
	} while (changed != false);
}

bool MapGenerator::CheckCurve(bool s[3][3])
{
	if (s[0][0] == true && s[1][0] == true &&	
		s[0][1] == true &&						s[2][1] == false &&
						   s[1][2] == false) 
		return true;

	if (					 s[1][0] == true &&	s[2][0] == true &&
		s[0][1] == false &&						s[2][1] == true &&
			                 s[1][2] == false) 
		return true;

	if (					s[1][0] == false && 
		s[0][1] == false &&						s[2][1] == true &&
							s[1][2] == true &&	s[2][2] == true)
		return true;

	if (				   s[1][0] == false && 
		s[0][1] == true &&						s[2][1] == false &&
		s[0][2] == true && s[1][2] == true) 
		return true;

	return false;
}

bool MapGenerator::Check3way(bool s[3][3])
{
	if (s[0][0] == true && s[1][0] == true && s[2][0] == true &&
		s[0][1] == true &&						s[2][1] == true &&
						   s[1][2] == false							) return true;

	if (					s[1][0] == true && s[2][0] == true &&
		s[0][1] == false &&					   s[2][1] == true &&
							s[1][2] == true && s[2][2] == true) return true;

	if (				   s[1][0] == false && 
		s[0][1] == true &&							s[2][1] == true &&
		s[0][2] == true && s[1][2] == true && s[2][2] == true) return true;

	if (s[0][0] == true && s[1][0] == true && 
		s[0][1] == true &&						s[2][1] == false &&
		s[0][2] == true && s[1][2] == true ) return true;

	return false;
}

void MapGenerator::Make()
{
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			Pt curr = Pt(x, y);
			if (GetBlock(curr) != -2) continue;
			bool u = ShouldConnectUp(curr);
			bool d = ShouldConnectDown(curr);
			bool l = ShouldConnectLeft(curr);
			bool r = ShouldConnectRight(curr);

			blocks[x + y * sizeX] = ChooseBlock(u, d, r, l);
		}
	}
}

bool MapGenerator::ShouldConnectUp(Pt current)
{
	current.y--;
	if (!InRange(current)) return false;


	int block = GetBlock(current);
	if (block == 0) return false;
	else if (block == 1) return true;
	else if (block == 2) return false;
	else if (block == 3) return false;
	else if (block == 4) return false;
	else if (block == 5) return true;
	else if (block == 6) return false;
	else if (block == 7) return false;
	else if (block == 8) return true;
	else if (block == 9) return false;
	else if (block == 10) return true;
	else if (block == 11) return true;
	else if (block == 12) return true;
	else if (block == 13) return true;
	else if (block == 14) return false;
	else if (block == 15) return true;
	else if (block == ROAD_IN_PROGRESS) return true;

	return false;
}

bool MapGenerator::ShouldConnectDown(Pt current)
{
	current.y++;
	if (!InRange(current)) return false;


	int block = GetBlock(current);
	if (block == 0) return false;
	else if (block == 1) return false;
	else if (block == 2) return false;
	else if (block == 3) return true;
	else if (block == 4) return false;
	else if (block == 5) return false;
	else if (block == 6) return true;
	else if (block == 7) return true;
	else if (block == 8) return false;
	else if (block == 9) return false;
	else if (block == 10) return true;
	else if (block == 11) return true;
	else if (block == 12) return false;
	else if (block == 13) return true;
	else if (block == 14) return true;
	else if (block == 15) return true;
	else if (block == ROAD_IN_PROGRESS) return true;

	return false;
}

bool MapGenerator::ShouldConnectLeft(Pt current)
{
	current.x--;
	if (!InRange(current)) return false;


	int block = GetBlock(current);
	if (block == 0) return false;
	else if (block == 1) return false;
	else if (block == 2) return false;
	else if (block == 3) return false;
	else if (block == 4) return true;
	else if (block == 5) return true;
	else if (block == 6) return true;
	else if (block == 7) return false;
	else if (block == 8) return false;
	else if (block == 9) return true;
	else if (block == 10) return false;
	else if (block == 11) return true;
	else if (block == 12) return true;
	else if (block == 13) return false;
	else if (block == 14) return true;
	else if (block == 15) return true;
	else if (block == ROAD_IN_PROGRESS) return true;

	return false;
}

bool MapGenerator::ShouldConnectRight(Pt current)
{
	current.x++;
	if (!InRange(current)) return false;


	int block = GetBlock(current);
	if (block == 0) return false;
	else if (block == 1) return false;
	else if (block == 2) return true;
	else if (block == 3) return false;
	else if (block == 4) return false;
	else if (block == 5) return false;
	else if (block == 6) return false;
	else if (block == 7) return true;
	else if (block == 8) return true;
	else if (block == 9) return true;
	else if (block == 10) return false;
	else if (block == 11) return false;
	else if (block == 12) return true;
	else if (block == 13) return true;
	else if (block == 14) return true;
	else if (block == 15) return true;
	else if (block == ROAD_IN_PROGRESS) return true;

	return false;
}

int MapGenerator::ChooseBlock(bool up, bool down, bool right, bool left)
{
	if (up == false && down == false && right == false && left == false) return 0;

	else if (up == false && down == true && right == false && left == false) return 1;
	else if (up == false && down == false && right == false && left == true) return 2;
	else if (up == true && down == false && right == false && left == false) return 3;
	else if (up == false && down == false && right == true && left == false) return 4;

	else if (up == false && down == true && right == true && left == false) return 5;
	else if (up == true && down == false && right == true && left == false) return 6;
	else if (up == true && down == false && right == false && left == true) return 7;
	else if (up == false && down == true && right == false && left == true) return 8;

	else if (up == false && down == false && right == true && left == true) return 9;
	else if (up == true && down == true && right == false && left == false) return 10;

	else if (up == true && down == true && right == true && left == false) return 11;
	else if (up == false && down == true && right == true && left == true) return 12;
	else if (up == true && down == true && right == false && left == true) return 13;
	else if (up == true && down == false && right == true && left == true) return 14;

	else if (up == true && down == true && right == true && left == true) return 15;

	return 0;
}

bool MapGenerator::InRange(Pt point)
{
	if (point.x < 0 || point.x >= sizeX) return false;
	if (point.y < 0 || point.y >= sizeY) return false;
	return true;
}

float MapGenerator::Distance(Pt A, Pt B)
{
	float ret;

	ret = sqrt(pow(B.x - A.x, 2) + pow(B.y - A.y, 2));

	return ret;
}

void MapGenerator::Connect(Pt A, Pt B)
{
	if (A.x == B.x && A.y == B.y) return;

	int *pathMap = new int[sizeX*sizeY];
	int finishIndex = B.x + B.y * sizeX;
	std::list<Pt> placesToVisit;
	Pt currentPlace;
	currentPlace.x = A.x;
	currentPlace.y = A.y;

	for (int i = 0; i < sizeX*sizeY; i++) pathMap[i] = MAX_INT_VALUE;
	pathMap[A.x + A.y * sizeX] = 0;
	placesToVisit.push_back(A);

	do
	{
		if (placesToVisit.size() == 0)
		{
			std::cout << "Something went wrong. Cannot connect points " << A.x << "-" << A.y << " and " << B.x << "-" << B.y << "\n";
			return;
		}

		currentPlace = placesToVisit.front();
		placesToVisit.pop_front();

		bool canGoLeft = CanGoLeft(currentPlace);
		bool canGoRight = CanGoRight(currentPlace);
		bool canGoUp = CanGoUp(currentPlace);
		bool canGoDown = CanGoDown(currentPlace);

		int leftIndex = currentPlace.x - 1 + currentPlace.y * sizeX;
		int rightIndex = currentPlace.x + 1 + currentPlace.y * sizeX;
		int upIndex = currentPlace.x + (currentPlace.y - 1) * sizeX;
		int downIndex = currentPlace.x + (currentPlace.y + 1) * sizeX;

		int blockValue = pathMap[currentPlace.x + currentPlace.y * sizeX];

		if (canGoLeft && pathMap[leftIndex] > blockValue + LeftMul(currentPlace))
		{
			if (leftIndex == finishIndex)
			{
				pathMap[finishIndex] = blockValue + LeftMul(currentPlace);
				currentPlace.x--;
				break;
			}

			pathMap[leftIndex] = blockValue + LeftMul(currentPlace);
			placesToVisit.push_back(Pt(currentPlace.x - 1, currentPlace.y));
		}

		if (canGoRight && pathMap[rightIndex] > blockValue + RightMul(currentPlace))
		{
			if (leftIndex == finishIndex)
			{
				pathMap[finishIndex] = blockValue + RightMul(currentPlace);
				currentPlace.x++;
				break;
			}

			pathMap[rightIndex] = blockValue + RightMul(currentPlace);
			placesToVisit.push_back(Pt(currentPlace.x + 1, currentPlace.y));
		}

		if (canGoUp && pathMap[upIndex] > blockValue + UpMul(currentPlace))
		{
			if (leftIndex == finishIndex)
			{
				pathMap[finishIndex] = blockValue + UpMul(currentPlace);
				currentPlace.y--;
				break;
			}

			pathMap[upIndex] = blockValue + UpMul(currentPlace);
			placesToVisit.push_back(Pt(currentPlace.x, currentPlace.y - 1));
		}

		if (canGoDown && pathMap[downIndex] > blockValue + DownMul(currentPlace))
		{
			if (leftIndex == finishIndex)
			{
				pathMap[finishIndex] = blockValue + DownMul(currentPlace);
				currentPlace.y++;
				break;
			}

			pathMap[downIndex] = blockValue + DownMul(currentPlace);
			placesToVisit.push_back(Pt(currentPlace.x, currentPlace.y + 1));
		}

	} while (currentPlace.x != B.x || currentPlace.y != B.y);


	do
	{
		int dirs[] =
		{
			pathMap[currentPlace.x - 1 + currentPlace.y * sizeX], //left
			pathMap[currentPlace.x + 1 + currentPlace.y * sizeX], //right
			pathMap[currentPlace.x + (currentPlace.y - 1) * sizeX], //up
			pathMap[currentPlace.x + (currentPlace.y + 1) * sizeX] //down
		};

		int smallest = dirs[0];
		int index = 0;

		for (int i = 1; i < 4; i++)
		{
			if (smallest > dirs[i])
			{
				smallest = dirs[i];
				index = i;
			}
		}

		blocks[currentPlace.x + currentPlace.y * sizeX] = ROAD_IN_PROGRESS;

		switch (index)
		{
		case 0:
			currentPlace.x--;
			break;

		case 1:
			currentPlace.x++;
			break;

		case 2:
			currentPlace.y--;
			break;

		case 3:
			currentPlace.y++;
			break;
		}

		//std::cout << currentPlace.x << " " << currentPlace.y << "\n";

	} while (currentPlace.x != A.x || currentPlace.y != A.y);

	blocks[currentPlace.x + currentPlace.y * sizeX] = ROAD_IN_PROGRESS;

	delete[] pathMap;
}


void MapGenerator::CreateBuildings()
{
	for (int i = 0; i < sizeX * sizeY; i++)
	{
		if (blocks[i] == 0)
		{
			int height = rand() % 4 + 2; //min : 1; max : 5; meaning that building will have 1 to 5 additional floors
			int groundFloor = rand() % this->numGroundFloors;

			this->buildings[i].push_back(groundFloor);

			for (int j = 1; j <= height; j++)
			{
				int floor = rand() % this->numHigherFloors;
				this->buildings[i].push_back(floor);
			}
		}
	}
}

int MapGenerator::LeftMul(Pt current)
{
	if (!CanGoLeft(current)) return 100000;
	else if (blocks[current.x - 1 + current.y * sizeX] == ROAD_IN_PROGRESS)
	{
		return 1;
	}
	else return 10;
}

int MapGenerator::RightMul(Pt current)
{
	if (!CanGoRight(current)) return 100000;
	else if (blocks[current.x + 1 + current.y * sizeX] == ROAD_IN_PROGRESS)
	{
		return 1;
	}
	else return 10;
}

int MapGenerator::UpMul(Pt current)
{
	if (!CanGoUp(current)) return 100000;
	else if (blocks[current.x + (current.y - 1) * sizeX] == ROAD_IN_PROGRESS)
	{
		return 1;
	}
	else return 10;
}

int MapGenerator::DownMul(Pt current)
{
	if (!CanGoDown(current)) return 100000;
	else if (blocks[current.x + (current.y + 1) * sizeX] == ROAD_IN_PROGRESS)
	{
		return 1;
	}
	else return 10;
}

int MapGenerator::GetBlock(Pt point)
{
	const static int invalid = 0x80000000;
	if (point.x < 0) return invalid;
	if (point.x >= sizeX) return invalid;
	if (point.y < 0) return invalid;
	if (point.y >= sizeY) return invalid;
	return blocks[point.x + point.y * sizeX];
}

void MapGenerator::Generate()
{
	BuildArrays();

	bool fail;

	std::vector<Pt> pts;

	srand(time(NULL));
	GenerateBasePavements();
	GenerateSpecialBuildings();

	for (int i = 0; i < points; i++)
	{
		fail = false;
		Pt randomPoint;
		randomPoint.x = rand() % (sizeX - 2) + 1;
		randomPoint.y = rand() % (sizeY - 2) + 1;

		for (int i = 0; i < pts.size(); i++)
		{
			if (Distance(randomPoint, pts[i]) < minimumDistance)
			{

				fail = true;
				break;
			}
		}

		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				Pt point = Pt(randomPoint.x + i, randomPoint.y + j);
				if (InRange(point) && GetBlock(point) == ROAD_IN_PROGRESS)
				{
					fail = true;
				}
			}
		}

		if (fail)
		{
			i--;
			continue;
		}


		if (pts.size() > 0)
		{
			int connections = rand() % 3;
			for (int i = 0; i < max((int)pts.size(), connections); i++)
			{
				int randomIndex = rand() % pts.size();
				Connect(randomPoint, pts[randomIndex]);
			}
		}

		pts.push_back(randomPoint);
	}

	for (int i = 0; i < PointsToConnect.size(); i++)
	{
		int r = rand() % pts.size();
		Connect(Pt(PointsToConnect[i].x, PointsToConnect[i].y), pts[r]);
	}

	this->DeleteUnneceseary();
	this->CleanUp();

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			if (GetBlock(Pt(x, y)) == ROAD_IN_PROGRESS) std::cout << "X";
			else if (GetBlock(Pt(x, y)) == 1000) std::cout << "+";
			else if (GetBlock(Pt(x, y)) == 0) std::cout << "@";
			else if (GetBlock(Pt(x, y)) == 1001) std::cout << "o";
			else std::cout << " ";
		}
		std::cout << "\n";
	}
	this->Make();
	this->CreateBuildings();

}

void MapGenerator::GenerateBasePavements()
{
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			if (x == 0 || x == sizeX - 1) blocks[x + y * sizeX] = 0;
			if (y == 0 || y == sizeY - 1) blocks[x + y * sizeX] = 0;
		}
	}

	std::cout << "PAVEMENT GENERATION\n";
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			if (GetBlock(Pt(x, y)) == ROAD_IN_PROGRESS) std::cout << "X";
			else if (GetBlock(Pt(x, y)) == 1000) std::cout << "+";
			else if (GetBlock(Pt(x, y)) == 0) std::cout << "@";
			else if (GetBlock(Pt(x, y)) == 1001) std::cout << "o";
			else std::cout << " ";
		}
		std::cout << "\n";
	}
}