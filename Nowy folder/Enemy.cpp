#include "Enemy.h"

float Enemy::closest_wall_distance(Maze* maze, sf::Vector2f pos, float size)
{
	sf::Vector2f position = pos;
	float closest_wall = 50.f;
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			int cellX = static_cast<int>((position.x) / 50.f) + i;
			int cellY = static_cast<int>((position.y) / 50.f) + j;
			Cell cell = maze->getCell(cellY, cellX);

			if (cell == Cell::Wall)
			{
				float distance = Maze::Cell_SDF(sf::Vector2f(position.x, position.y), sf::Vector2f(cellX * 50.f, cellY * 50.f), size);
				if (distance < closest_wall)
				{
					closest_wall = distance;
				}
			}
		}
	}
	return closest_wall;
}
sf::Vector2f Enemy::raycast(sf::Vector2f start_pos, float angle, float distance)
{
	float x_distance = -sin(deg_to_rad(angle)) * distance;
	float y_distance = cos(deg_to_rad(angle)) * distance;

	sf::Vector2f end_pos = sf::Vector2f(start_pos.x + x_distance, start_pos.y + y_distance);
	return end_pos;
}

Enemy::Enemy() : body(Vector2f(25.f, 25.f)), health(100.f)
{
	this->texture.loadFromFile("enemy.png");
	this->body.setTexture(&texture);
	this->fov = sf::CircleShape(200.f, 3);

	this->ray = sf::CircleShape(2.f);
	this->ray.setFillColor(sf::Color::Red);
}

Enemy::~Enemy() {}

void Enemy::Render(sf::RenderWindow& window)
{
	float x = -cos(deg_to_rad(rotation)) * 200;
	float y = -sin(deg_to_rad(rotation)) * 200;
	fov.setPosition(body.getPosition().x + x + 12.5f, body.getPosition().y + y + 12.5f);
	fov.setRotation(rotation);

	window.draw(body);
	window.draw(fov);
	//window.draw(ray);
}

void Enemy::Update(Maze* maze)
{
	float ray_distance = 0.f;
	sf::Vector2f target_pos = player_pos + sf::Vector2f(12.5f, 12.5f);
	sf::Vector2f start_pos = body.getPosition() + sf::Vector2f(body.getSize().x / 2, body.getSize().y / 2);
	int steps = 0;
	const int MAX_STEPS = 1000;
	float target_distance = length(start_pos - target_pos);
	float closest_wall = closest_wall_distance(maze, start_pos, 0);
	ray_distance = std::min(target_distance, closest_wall);
	while (ray_distance != 0.f && steps < MAX_STEPS)
	{
		target_distance = length(start_pos - target_pos);
		closest_wall = closest_wall_distance(maze, start_pos, 0);
		ray_distance = std::min(target_distance, closest_wall);
		if (ray_distance == target_distance)
		{
			ray.move(target_pos - start_pos);
			break;
		}
		start_pos = raycast(start_pos, rotation, ray_distance);
		ray.setPosition(start_pos);
		steps++;
	}

	if (player_spotted)
	{
		float x = player_pos.x - body.getPosition().x;
		float y = player_pos.y - body.getPosition().y;

		if (x == 0 || y == 0)
			return;

		float angle = -atan2(y, x) * 180 / 3.14159265;

		rotation = 270 - angle;
	}


	if (ray_distance == target_distance)
	{
		follow_player = true;
	}
	else
	{
		follow_player = false;
	}

	if (follow_player)
	{
		float closest_wall = closest_wall_distance(maze, body.getPosition(), 12.5f);
		sf::Vector2f start_pos = body.getPosition() + sf::Vector2f(body.getSize().x / 2, body.getSize().y / 2);

		body.setPosition(body.getPosition().x + -sin(deg_to_rad(rotation)) * 0.1f, body.getPosition().y + cos(deg_to_rad(rotation)) * 0.1f);
		fov.setFillColor(sf::Color(255, 0, 0, 100)); 
	}

	else
	{
		fov.setFillColor(sf::Color(0, 255, 0, 100));

		explore_maze(maze);

		if (change_path_clock.getElapsedTime().asSeconds() >= target_time)
		{
			change_path(maze);
		}
		int x1 = floor(this->body.getPosition().x * 20 / 1000);
		int y1 = floor(this->body.getPosition().y * 20 / 1000);
		if (x1 == target_pos.x && y1 == target_pos.y)
		{
			change_path(maze);
		}
	}
}

void Enemy::change_path(Maze* maze)
{
	std::uniform_int_distribution<> change_path_time(1, 10);
	std::uniform_int_distribution<> pos(0, maze->floors.size() - 1);
	std::mt19937 gen(std::time(0) + reinterpret_cast<int>(this));
	int time = change_path_time(gen);
	int index = pos(gen);
	target_pos = maze->floors[index];

	//std::cout << "Last time: " << target_time << ' ' << time << '\n' << target_pos.x << ' ' << target_pos.y << '\n';
 	target_time = time;
	change_path_clock.restart();
}

void Enemy::explore_maze(Maze* maze)
{
	int x1 = floor(this->body.getPosition().x * 20 / 1000);
	int y1 = floor(this->body.getPosition().y * 20 / 1000);
	std::queue<int> q = Maze::show_path(x1, y1, this->target_pos.x, this->target_pos.y, maze->vp);
	if (q.size() > 1) {
		int x2 = (q.front() % 20);
		int y2 = floor(q.front() / 20);
		if (x1 == x2 && y2 == y1) {
			q.pop();
			x2 = (q.front() % 20);
			y2 = floor(q.front() / 20);
		}
		//std::cout << "x1 y1 " << x1 << " " << y1 << "\n ";
		//std::cout << "x2 y2 " << x2 << " " << y2 << "\n ";
		//std::cout << '\n';
		float x1_ = this->body.getPosition().x;
		float y1_ = this->body.getPosition().y;
		float x2_ = x2 * 50+25;
		float y2_ = y2 * 50+25;
		float dx = (x2_ - x1_);
		float dy = (y2_ - y1_);
		float dr = sqrt(dx * dx + dy * dy);
		float speed = 0.2;
		if (dr > 0) {
			if (dy * dx != 0) {
				float angle = -atan2(dy, dx) * 180 / 3.14159265;
				rotation = 270 - angle;
			}
			body.setPosition(body.getPosition().x + dx * speed / dr, body.getPosition().y + dy * speed / dr);
		}
	}
}

bool Enemy::is_in_fov(Vector2f target_pos)
{
	float origin_x = body.getPosition().x + 12.5f;
	float origin_y = body.getPosition().y + 12.5f;
	float target_x = target_pos.x + 12.5f;
	float target_y = target_pos.y + 12.5f;

	float x = target_x - origin_x;
	float y = target_y - origin_y;

	if (sqrt(x * x + y * y) > DETECTION_RADIUS)
	{
		player_pos = target_pos;
		player_spotted = false;
		return false;
	}

	float cos_angle = cos(deg_to_rad(rotation));
	float sin_angle = -sin(deg_to_rad(rotation));

	float x_cos = x * cos_angle;
	float x_sin = x * sin_angle;
	float y_cos = y * cos_angle;
	float y_sin = y * sin_angle;

	float lhs = x_cos - y_sin;
	float rhs = x_sin + y_cos;

	if (lhs > -rhs && lhs < rhs)
	{
		player_pos = target_pos;
		player_spotted = true;
		return true;
	}
	player_pos = target_pos;
	player_spotted = false;
	return false;
}
