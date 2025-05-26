#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <random>

#include "Enemy.h"
#include "Maze.h"
#include "Player.h"

using namespace sf;

struct GUI
{
	sf::RectangleShape OuterOuter;
	sf::RectangleShape Outer;
	sf::RectangleShape Inner;
	sf::Text Score;
	sf::Text Health;
	sf::Text Level;
};

class Game
{
private: 
	Event event;
	VideoMode videomode;

	void initializeVariables();
	void initializeWindow();
	void initializeEnemies();
	void initializeMaze();
	void initializeGUI();
public:
	GUI gamegui;
	RenderWindow* window;
	Maze* maze;
	Player* player;
	Font font;

	Game();
	~Game();

	void renderObjects() {};
	void renderGUI(Player* player);
	void update();
	void render();
	void pollEvents();

	const bool running() const;
};

