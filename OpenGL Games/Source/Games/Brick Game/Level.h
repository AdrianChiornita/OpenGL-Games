#pragma once

#include "O2D.h"
#include "Object.h"

#include <vector>
#include <include/glm.h>
#include <Core/Engine.h>

typedef struct BrickStructure {
	int realbricks;
	int totalbricks;
	int numberofbricksperline;
	int numberoflines;
	std::vector<std::vector<Brick *>> bricks;
} BrickStructure;

class Level 
{
private:
	int number;
	
	static Ball* ball;
	BrickStructure brickstruct;

	static Platform* platform;
	std::vector<Wall *> walls;

public:

	
	static glm::ivec2 resolution;
	static const int MAXLEV;

	Level(int number);
	~Level() {}

	static Ball* getBall();
	static Platform* getPlatform();
	BrickStructure getObstacles();
	std::vector<Wall *> getWalls();
};