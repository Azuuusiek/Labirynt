#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include <queue>

#include "Maze.h"

class Player {
private:
    bool canMove(float x, float y, Maze* maze1);
    sf::Texture texture;
    sf::Texture bullet_texture;
    sf::Texture step_texture;
public:
    int health;
    int score;
    int level;
    float speed;

    void damagePlayer(int damage, Maze* maze);
    void healPlayer();
    void tookDamage(bool damage);
    sf::RectangleShape body;
    Maze& maze;
    sf::RectangleShape newShape;
    sf::RectangleShape Filter;
    bool immunityCheck;
    bool showdamagefilter;
    sf::Clock immunityClock;
    sf::Clock damageClock;
    Player(float size, float speed, Maze& maze);
    void handleInput(sf::RenderWindow* window, Maze* maze, sf::Vector2i mousePosition);
    void damage_enemy();
    void render(sf::RenderWindow& window);
    void showpath(int x, int y);
};