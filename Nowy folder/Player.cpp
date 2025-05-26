#include "Player.h"
#include "Maze.h"
#include "Enemy.h"

bool t = true;
bool vis[10000];
std::queue<int> QQ, QQ2;
float aim_lenght = 12.5;
bool left_pressed = false;
bool key_found = false;
bool show = false;
float dx, dy, r, vx, vy;
float nx = 0, ny = 0;
float bullet_speed = 1;
float bullet_size = 5;

struct bullet
{
    float x;
    float y;
    float vx;
    float vy;
    float t;
};
std::vector<bullet> bullets;
struct Aim
{
    float x;
    float y;
};

Aim aimm;
float angle;

void Player::damage_enemy() {
    std::vector<Enemy*> vv = this->maze.enemy_list;
    std::queue<int> q_;
    int k = 0;
    for (int i = 0; i < vv.size(); i++) {
        sf::Vector2f pos = vv[i]->body.getPosition();

        for (int j = 0; j < bullets.size(); j++) {
            bullet b_ = bullets[j];
            if (b_.x + bullet_size >= pos.x && b_.x <= pos.x + 25 && b_.y + bullet_size >= pos.y && b_.y <= pos.y + 25) {
                bullets.erase(bullets.begin() + j);
                j--;
                //std::cout << "hiy" << '\n';
                vv[i]->health -= 11;
                if (vv[i]->health < 0) {
                    vv.erase(vv.begin() + i);
                    i--;
                }
            }
        }
    }
    this->maze.enemy_list = vv;
}


void add_bullet(sf::Vector2f pos1, sf::Vector2i pos2, Maze& maze) {
    if (vx != 0 && vy != 0) {
        float a = 3.14 * 1.5 - angle;
        float x = cos(a) * aim_lenght * 2 + pos1.x + 12.5;
        float y = -sin(a) * aim_lenght * 2 + pos1.y + 12.5;
        if (maze.getCell(floor(y * 20 / 1000), floor(x * 20 / 1000)) != Cell::Wall) {
            bullets.push_back({ x,y,vx,vy,0 });
        }
    }
}

bullet move_bullet(bullet bull, Maze& maze) {
    bullet b = bull;
    int X0 = floor((bull.x) * 20 / 1000);
    int Y0 = floor((bull.y) * 20 / 1000);
    int X = floor((bull.x + bull.vx * bullet_speed) * 20 / 1000);
    int Y = floor((bull.y + bull.vy * bullet_speed) * 20 / 1000);
    int X2 = floor((bull.x + bull.vx * bullet_speed + bullet_size) * 20 / 1000);
    int Y2 = floor((bull.y + bull.vy * bullet_speed + bullet_size) * 20 / 1000);
    if (maze.getCell(Y0, X2) == Cell::Wall || maze.getCell(Y0, X) == Cell::Wall) {
        b.vx *= -1;
    }
    if (maze.getCell(Y2, X0) == Cell::Wall || maze.getCell(Y, X0) == Cell::Wall) {
        b.vy *= -1;
    }
    return b;
}

void Player::showpath(int x, int y) {
    QQ = QQ2;
    //QQ_ = QQ2_;
    int edge = y * 20 + x;
    int exit;
    if (this->maze.key_found) {
        exit = 19 * 20 + 19;
    }
    else {
        exit = this->maze.key_position;
    }
    std::queue<std::pair<int, std::queue<int> > > q;
   // std::vector<std::pair<int, std::vector<int> > > q_;
    std::queue<int> k;
    //std::vector<int> k_;
  //  q_.push_back({ edge,k_ });
    q.push({ edge,k });
    while (!q.empty()) {
        int w = q.front().first;
        std::queue<int> Q = q.front().second;
       // std::vector<int> Q_ = q_.front().second;
        Q.push(w);
        q.pop();
        for (int i = 0; i < this->maze.vp[w].size(); i++) {
            if (!vis[this->maze.vp[w][i]]) {
                vis[this->maze.vp[w][i]] = true;
                q.push({ this->maze.vp[w][i], Q });
                if (this->maze.vp[w][i] == exit) {
                    QQ = Q;
                    for (int j = 0; j < 1000; j++) {
                        vis[j] = false;
                    }
                    goto end;
                }
            }
        }
    }
end:
    if (QQ.size() > 0) {
        QQ.pop();
    }
}

Player::Player(float size, float speed, Maze& maze) : body(sf::Vector2f(size, size)), speed(speed), maze(maze), immunityCheck(false), health(100), score(0), level(0)
{
    this->texture.loadFromFile("player2.png");
	this->bullet_texture.loadFromFile("bullet.png");
    this->body.setTexture(&texture);
	this->step_texture.loadFromFile("step.png");
    body.setPosition(51.0f, 51.0f); // Starting position
}


bool Player::canMove(float x, float y, Maze* maze1)
{
    //Getting player position and contacted cell
    sf::Vector2f position = body.getPosition();
    int cellX = static_cast<int>((position.x + x) * 20 / 1000);
    int cellY = static_cast<int>((position.y + y) * 20 / 1000);
    int cellX2 = static_cast<int>((position.x + x + 25.0f) * 20 / 1000);
    int cellY2 = static_cast<int>((position.y + y + 25.0f) * 20 / 1000);
    float X = (position.x + x) * 20 / 1000;
    float Y = (position.y + y) * 20 / 1000;
    float X2 = (position.x + x + 25) * 20 / 1000;
    float Y2 = (position.y + y + 25) * 20 / 1000;

    //Behavior depending on the cell type, repeated for each corner of the player
    if (maze.getCell(cellY, cellX) == Cell::Wall) {
        newShape = sf::RectangleShape(sf::Vector2f(50.0f, 50.0f));
        newShape.setFillColor(sf::Color::Red);
        newShape.setPosition(floor(X) * 1000 / 20, floor(Y) * 1000 / 20);
        // std::cout<<"cellX "<<cellX<<" cellY "<<cellY<<" "<<
    }
    else if (maze.getCell(cellY, cellX) == Cell::End) {
        this->level = +1;
        body.setPosition(51.0f, 51.0f);
        delete maze1;
        maze1 = new Maze(21, 21, this);
        maze1->generateMaze();
    }
    else if (maze.getCell(cellY, cellX) == Cell::Key) {
        maze1->unlocked = true;
        maze1->grid[cellY][cellX] = Cell::Passage;
		maze.key_found = true;
    }
    else if (maze.getCell(cellY, cellX) == Cell::Elixer)
    {
        healPlayer();
        maze1->grid[cellY][cellX] = Cell::Passage;
    }
    else if (maze.getCell(cellY, cellX) == Cell::SpikeFloor)
    {
        for (int i = 0; i < maze1->trapcount; i++)
        {
            if (maze1->spikelist[i].xposition == cellX && maze1->spikelist[i].yposition == cellY)
            {
                if (maze1->spikelist[i].discovered == false)
                {
                    maze1->spikelist[i].clock.restart();
                    maze1->spikelist[i].discovered = true;
                }
                else
                {
                    if (maze1->spikelist[i].clock.getElapsedTime() >= sf::seconds(1.0f))
                    {
                        damagePlayer(maze1->spikelist[i].damagePool, maze1);
                    }
                }
            }
        }
    }
    else if (maze.getCell(cellY, cellX) == Cell::Trophy)
    {
        score += 100;
        maze1->grid[cellY][cellX] = Cell::Passage;
    }

    if (maze.getCell(cellY2, cellX) == Cell::Wall) {
        newShape = sf::RectangleShape(sf::Vector2f(50.0f, 50.0f));
        newShape.setFillColor(sf::Color::Red);
        newShape.setPosition(floor(X) * 1000 / 20, floor(Y) * 1000 / 20);
    }
    else if (maze.getCell(cellY2, cellX) == Cell::End) {
        this->level = +1;
        body.setPosition(51.0f, 51.0f);
        delete maze1;
        maze1 = new Maze(21, 21, this);
        maze1->generateMaze();
    }
    else if (maze.getCell(cellY2, cellX) == Cell::Key) {
        maze1->unlocked = true;
        maze1->grid[cellY2][cellX] = Cell::Passage;
        maze.key_found = true;
    }
    else if (maze.getCell(cellY2, cellX) == Cell::Elixer)
    {
        healPlayer();
        maze1->grid[cellY2][cellX] = Cell::Passage;
    }
    else if (maze.getCell(cellY2, cellX) == Cell::SpikeFloor)
    {
        for (int i = 0; i < maze1->trapcount; i++)
        {
            if (maze1->spikelist[i].xposition == cellX && maze1->spikelist[i].yposition == cellY2)
            {
                if (maze1->spikelist[i].discovered == false)
                {
                    maze1->spikelist[i].clock.restart();
                    maze1->spikelist[i].discovered = true;
                }
                else
                {
                    if (maze1->spikelist[i].clock.getElapsedTime() >= sf::seconds(1.0f))
                    {
                        damagePlayer(maze1->spikelist[i].damagePool, maze1);
                    }
                }
            }
        }
    }
    else if (maze.getCell(cellY2, cellX) == Cell::Trophy)
    {
        score += 100;
        maze1->grid[cellY2][cellX] = Cell::Passage;
    }

    if (maze.getCell(cellY2, cellX2) == Cell::Wall) {
        newShape = sf::RectangleShape(sf::Vector2f(50.0f, 50.0f));
        newShape.setFillColor(sf::Color::Red);
        newShape.setPosition(floor(X2) * 1000 / 20, floor(Y2) * 1000 / 20);
    }
    else if (maze.getCell(cellY2, cellX2) == Cell::End) {
        this->level = +1;
        body.setPosition(51.0f, 51.0f);
        delete maze1;
        maze1 = new Maze(21, 21, this);
        maze1->generateMaze();
    }
    else if (maze.getCell(cellY2, cellX2) == Cell::Key) {
        maze1->unlocked = true;
        maze1->grid[cellY2][cellX2] = Cell::Passage;
		maze.key_found = true;
    }
    else if (maze.getCell(cellY2, cellX2) == Cell::Elixer)
    {
        healPlayer();
        maze1->grid[cellY2][cellX2] = Cell::Passage;
    }
    else if (maze.getCell(cellY2, cellX2) == Cell::SpikeFloor)
    {
        for (int i = 0; i < maze1->trapcount; i++)
        {
            if (maze1->spikelist[i].xposition == cellX2 && maze1->spikelist[i].yposition == cellY2)
            {
                if (maze1->spikelist[i].discovered == false)
                {
                    maze1->spikelist[i].clock.restart();
                    maze1->spikelist[i].discovered = true;
                }
                else
                {
                    if (maze1->spikelist[i].clock.getElapsedTime() >= sf::seconds(0.5f))
                    {
                        damagePlayer(maze1->spikelist[i].damagePool, maze1);
                    }
                }
            }
        }
    }
    else if (maze.getCell(cellY2, cellX2) == Cell::Trophy)
    {
        score += 100;
        maze1->grid[cellY2][cellX2] = Cell::Passage;
    }

    if (maze.getCell(cellY, cellX2) == Cell::Wall) {
        newShape = sf::RectangleShape(sf::Vector2f(50.0f, 50.0f));
        newShape.setFillColor(sf::Color::Red);
        newShape.setPosition(floor(X2) * 1000 / 20, floor(Y) * 1000 / 20);
    }
    else if (maze.getCell(cellY, cellX2) == Cell::End) {
        this->level = +1;
        body.setPosition(51.0f, 51.0f);
        delete maze1;
        maze1 = new Maze(21, 21, this);
        maze1->generateMaze();
    }
    else if (maze.getCell(cellY, cellX2) == Cell::Key) {
        maze1->unlocked = true;
        maze1->grid[cellY][cellX2] = Cell::Passage;
		maze.key_found = true;
    }
    else if (maze.getCell(cellY, cellX2) == Cell::Elixer)
    {
        healPlayer();
        maze1->grid[cellY][cellX2] = Cell::Passage;
    }
    else if (maze.getCell(cellY, cellX2) == Cell::SpikeFloor)
    {
        for (int i = 0; i < maze1->trapcount; i++)
        {
            if (maze1->spikelist[i].xposition == cellX2 && maze1->spikelist[i].yposition == cellY)
            {
                if (maze1->spikelist[i].discovered == false)
                {
                    maze1->spikelist[i].clock.restart();
                    maze1->spikelist[i].discovered = true;
                }
                else
                {
                    if (maze1->spikelist[i].clock.getElapsedTime() >= sf::seconds(1.0f))
                    {
                        damagePlayer(maze1->spikelist[i].damagePool, maze1);
                    }
                }
            }
        }
    }
    else if (maze.getCell(cellY, cellX2) == Cell::Trophy)
    {
        score += 100;
        maze1->grid[cellY][cellX2] = Cell::Passage;
    }

    if (maze.getCell(cellY, cellX) != Cell::Wall && maze.getCell(cellY2, cellX2) != Cell::Wall && maze.getCell(cellY, cellX2) != Cell::Wall && maze.getCell(cellY2, cellX) != Cell::Wall)
    {
        if (maze.getCell(cellY, cellX) == Cell::Door || maze.getCell(cellY2, cellX2) == Cell::Door || maze.getCell(cellY, cellX2) == Cell::Door || maze.getCell(cellY2, cellX) == Cell::Door)
        {
            if (maze1->unlocked)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

void Player::damagePlayer(int damage, Maze* maze)
{
    if (!immunityCheck || immunityClock.getElapsedTime() >= sf::seconds(1.0f))
    {
        health = health - damage;

        tookDamage(true);

        immunityCheck = true;
        immunityClock.restart();

        if (health <= 0)
        {
            maze->enemynumber = 0;
            maze->generateMaze();
            body.setPosition(sf::Vector2f(51.0f, 51.0f));
            this->score = 0;
            this->level = 0;
            health = 100;
            return;
        }
    }

}

void Player::tookDamage(bool damage)
{
    showdamagefilter = true;
    damageClock.restart();
    Filter.setSize(sf::Vector2f(1000.0f, 1000.0f));

    if (damage)
    {
        Filter.setFillColor(sf::Color(255, 0, 0, 100));
    }
    else
    {
        Filter.setFillColor(sf::Color(0, 0, 0, 0));
    }
    return;

}

void Player::healPlayer()
{
    tookDamage(false);

	if (health = 100)
	{
		return;
	}
	else
	{
		health = health + 10;
		if (health > 100)
		{
			health = 100;
		}
	}
}

void showaim(sf::Vector2f pos1, sf::Vector2i pos2) {

    float x1 = pos1.x;
    float y1 = pos1.y;
    float x2 = pos2.x;
    float y2 = pos2.y;
    dx = x2 - x1;
    dy = y2 - y1;
    r = sqrt(dx * dx + dy * dy);
    angle = 0;
    vx = 0;
    vy = 0;
    if (r != 0) {
        if (dx > 0) {
            if (dy > 0) {
                angle = -asin(dx / r);
                vx = dx / r;
                vy = dy / r;
            }
            else {
                angle = -3.14 / 2 - acos(dx / r);
                vx = dx / r;
                vy = dy / r;
            }
        }
        else {
            if (dy > 0) {
                angle = acos(dy / r);
                vx = dx / r;
                vy = dy / r;
            }
            else {
                angle = 3.14 / 2 + asin(-dy / r);
                vx = dx / r;
                vy = dy / r;
            }
        }
    }
    aimm = { x1,y1 };
}

void Player::handleInput(sf::RenderWindow* window, Maze* maze, sf::Vector2i mousePosition)
{
	sf::Vector2f position = body.getPosition();
	float speed = 1.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && canMove(0, -speed, maze)) {
		body.move(0, -speed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && canMove(0, speed, maze)) {
		body.move(0, speed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && canMove(-speed, 0, maze)) {
		body.move(-speed, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && canMove(speed, 0, maze)) {
		body.move(speed, 0);
	}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
        int x = floor(body.getPosition().x * 20 / 1000);
        int y = floor(body.getPosition().y * 20 / 1000);
        if (x != nx || y != ny) {
            if (maze->key_found) {
                QQ = Maze::show_path(x, y, 19, 19, maze->vp);
            }
            else {
                QQ = Maze::show_path(x, y, maze->keypositionx, maze->keypositiony, maze->vp);
            }
        }
        nx = x;
        ny = y;
        show = true;
    }
    else {
        show = false;
    }
    showaim(body.getPosition(), mousePosition);
    damage_enemy();

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !left_pressed) {
        left_pressed = true;
        add_bullet(body.getPosition(), mousePosition, *maze);
    }
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        left_pressed = false;
    }

   // std::queue<bullet> bull;
    std::vector<bullet> bull;
    for (int i = 0; i < bullets.size(); i++) {
        bullet b = bullets[i];
        b = move_bullet(b, *maze);
        b.x += b.vx * bullet_speed;
        b.y += b.vy * bullet_speed;
        b.t++;
        if (b.t < 420) {
            bull.push_back(b);
        }
    }
    bullets = bull;
}

void Player::render(sf::RenderWindow& window)
{
    sf::RectangleShape block;
    block.setSize(sf::Vector2f(25.0f, 25.0f));
	block.setTexture(&step_texture);
    if (show) {
        QQ2 = QQ;
        while (!QQ2.empty()) {
            int x = QQ2.front() % 20;
            int y = QQ2.front() / 20;
            block.setPosition(sf::Vector2f(x * 50 + 10, y * 50 + 10));
            window.draw(block);
            QQ2.pop();
        }

    }
    sf::RectangleShape block2(sf::Vector2f(3.0f, 2 * aim_lenght));
    block2.setFillColor(sf::Color::White);

    // Ustaw pozycj� prostok�ta
    block2.setPosition(sf::Vector2f(aimm.x + 12.5, aimm.y + 12.5));

    // K�t obrotu w stopniach
    float rotationAngle = (angle) * 180 / 3.14;
    block2.setRotation(rotationAngle);
    window.draw(block2);

    sf::RectangleShape block3(sf::Vector2f(bullet_size, bullet_size));
	block3.setTexture(&bullet_texture);
    block3.setScale(2.f, 2.f);
    for (int i = 0; i < bullets.size();i++){
        block3.setPosition(sf::Vector2f(bullets[i].x, bullets[i].y));
        window.draw(block3);
    }
    if (showdamagefilter)
    {
        window.draw(Filter);
    }
    window.draw(body);
    //window.draw(newShape);
}