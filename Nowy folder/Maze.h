#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <random>
#include <queue>
#include <algorithm>

#include "vector_utils.h"

class Player;
class Enemy;

enum class Cell
{
    Wall,
    Passage,
    End,
    Door,
    Key,
    Elixer,
    SpikeFloor,
    Trophy
};

struct Room
{
    int x, y, width, height;
};

struct Spike
{
    bool discovered;
    int xposition;
    int yposition;
    int damagePool;
    sf::Clock clock;
};

class Maze
{
private:
    int width, height;
    
    std::vector<Room> rooms;

    std::random_device rd;
    std::mt19937 gen;

    sf::Texture wall_texture, key_texture, potion_texture, spike_texture, trophy_texture, portal_texture, door_texture, floor_texture;

    // Directions for carving passages
    const int dx[4] = { 1, -1, 0, 0 };
    const int dy[4] = { 0, 0, 1, -1 };

    void generatePassages(int x, int y);

public:
    std::vector<int> vp[10000];
    std::vector<Spike> spikelist;
    std::vector<Enemy*> enemy_list;
	Player* player;
    int trapcount;
    int keypositionx;
    int keypositiony;
    int key_position;
    bool unlocked;
    bool key_found = false;
    //bool vis[100000];
    //std::vector<int> v[100000];
    //void showpath(int x, int y);
    std::vector<std::vector<Cell>> grid;
    std::vector<sf::Vector2i> floors;
    int enemynumber;
    //static std::queue<int> QQ;
    void complete();
    Maze(int width, int height, Player* player);
    Cell getCell(int x, int y) const;
    void generateMaze();
    void handleInput(Player* player);
    void draw(sf::RenderWindow& window, float blockSize) const;
    const std::vector<Room>& getRooms() const;
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    void generate_floors();

    static float Cell_SDF(sf::Vector2f p, sf::Vector2f t, float size);
    static std::queue<int> show_path(int x1, int y1, int x2, int y2, std::vector<int> v[10000]);
};