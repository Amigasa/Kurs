#include "Player.h"
#include <cmath>
#include <iostream> // <-- ОБЯЗАТЕЛЬНО для std::cout

Player::Player() {
    position = { 1.5f, 1.5f };
    angle = 0.0f;
    health = MAX_HEALTH;
    ammo = MAX_AMMO;

    // Загрузка оружия
    if (weaponTexture.loadFromFile("Resources/shotgun.png")) {
        weaponSprite.setTexture(weaponTexture);
        weaponSprite.setScale(0.5f, 0.5f);
        weaponSprite.setPosition(SCREEN_WIDTH / 2.f - 100.f, SCREEN_HEIGHT - 200.f);
    }
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

    if (map[(int)position.y][(int)newX] == 0) position.x = newX;
    if (map[(int)newY][(int)position.x] == 0) position.y = newY;

    // --- ЛОГИКА СТРЕЛЬБЫ ---
    if (!canShoot) {
        shootTimer += dt;
        if (shootTimer >= SHOTGUN_COOLDOWN) {
            canShoot = true;
            shootTimer = 0.f;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canShoot && ammo > 0) {
        Shoot();
    }
}

// Реализация функции выстрела
void Player::Shoot() {
    ammo--;
    canShoot = false;
    std::cout << "BANG! Ammo left: " << ammo << std::endl;
}