#include "Player.h"
#include <cmath>

Player::Player() {
    position = { 3.5f, 3.5f };
    angle = 0.0f;
    health = MAX_HEALTH;
    ammo = MAX_AMMO;
}

void Player::Update(float dt, int map[MAP_HEIGHT][MAP_WIDTH]) {
    float moveSpeed = PLAYER_SPEED * dt;
    float rotSpeed = PLAYER_ROT_SPEED * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))  angle -= rotSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) angle += rotSpeed;

    float dx = cos(angle) * moveSpeed;
    float dy = sin(angle) * moveSpeed;

    float newX = position.x, newY = position.y;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { newX += dx; newY += dy; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { newX -= dx; newY -= dy; }

    // Коллизии со стенами
    if (map[(int)position.y][(int)newX] == 0) position.x = newX;
    if (map[(int)newY][(int)position.x] == 0) position.y = newY;
}