#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <random>
#include "Maze.h"

class Door
{
private:

public:
	bool unlocked;
	std::vector<Cell> top, left;
	Door(Maze* maze);
};

