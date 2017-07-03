// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <list>
#include <vector>
#include <time.h>
#include <algorithm>
#include <iostream>
#include "Building.h"

#define MAX_INT_VALUE 0b01111111111111111111111111111111
#define NOTHING -1
#define ROAD_IN_PROGRESS -2
#define RESERVED_FOR_ROAD -3

using namespace std;

class MapGenerator
{
public:
	MapGenerator();

	int sizeX;
	int sizeY;
	int points;
	int numGroundFloors;
	int numHigherFloors;

	int *blocks;
	bool *visible;
	bool *canDelete;

	std::vector<int> *buildings;

	float minimumDistance;

	void Generate();

	bool CanConnectLeft(int x, int y);
	bool CanConnectRight(int x, int y);
	bool CanConnectUp(int x, int y);
	bool CanConnectDown(int x, int y);

	std::vector<Building> buildingsInfo;

private:
	struct Pt
	{
		Pt()
		{
			x = 0;
			y = 0;
		}

		Pt(int _x, int _y)
		{
			this->x = _x;
			this->y = _y;
		}

		int x;
		int y;
	};

	std::vector<Pt> PointsToConnect;

	void GenerateBasePavements();
	void CleanUp();
	void DeleteUnneceseary();
	void Make();
	void Connect(Pt A, Pt B);
	void CreateBuildings();
	void GenerateSpecialBuildings();

	bool CanGoLeft(Pt current);
	bool CanGoRight(Pt current);
	bool CanGoDown(Pt current);
	bool CanGoUp(Pt current);
	bool InRange(Pt point);
	bool TryReserve(int x, int y, int width, int height, int building);

	bool ShouldConnectUp(Pt current);
	bool ShouldConnectDown(Pt current);
	bool ShouldConnectLeft(Pt current);
	bool ShouldConnectRight(Pt current);

	bool CheckCurve(bool s[3][3]);
	bool Check3way(bool s[3][3]);

	int LeftMul(Pt current);
	int RightMul(Pt current);
	int UpMul(Pt current);
	int DownMul(Pt current);
	int ChooseBlock(bool up, bool down, bool right, bool left);

	int GetBlock(Pt point);
	void SetBlock(Pt point, int value);
	void BuildArrays();

	float Distance(Pt A, Pt B);
	
};
