#include "Player.h"
#include <cmath>
#include <iostream>

Player::Player() {
    position = { 1.5f, 1.5f };
    angle = 0.0f;
    health = MAX_HEALTH;
    ammo = MAX_AMMO;

    // Загружаем текстуры ТОЛЬКО ОДИН РАЗ
    static sf::Texture weaponTex;
    static sf::Texture flashTex;
    static bool loaded = false;

    if (!loaded) {
        if (!weaponTex.loadFromFile("Resources/shotgun.png")) {
            std::cout << "Error: shotgun.png not found!" << std::endl;
        }
        if (!flashTex.loadFromFile("Resources/muzzle_flash.png")) {
            std::cout << "Error: muzzle_flash.png not found!" << std::endl;
        }
        loaded = true;
    }

    weaponSprite.setTexture(weaponTex);
    weaponSprite.setScale(0.5f, 0.5f);
    weaponSprite.setPosition(SCREEN_WIDTH / 2.f - 100.f, SCREEN_HEIGHT - 200.f);

    flashSprite.setTexture(flashTex);
    flashSprite.setScale(0.5f, 0.5f);
    flashSprite.setPosition(SCREEN_WIDTH / 2.f - 150.f, SCREEN_HEIGHT - 350.f);
}

void Player::Update(float dt, int map[MAP_HEIGHT][MAP_WIDTH]) {
    float moveSpeed = PLAYER_SPEED * dt;

    float dx = cos(angle) * moveSpeed;
    float dy = sin(angle) * moveSpeed;

    float strafeDx = cos(angle + 3.14159f / 2.f) * moveSpeed;
    float strafeDy = sin(angle + 3.14159f / 2.f) * moveSpeed;

    float newX = position.x, newY = position.y;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { newX += dx; newY += dy; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { newX -= dx; newY -= dy; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { newX -= strafeDx; newY -= strafeDy; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { newX += strafeDx; newY += strafeDy; }

    if (map[(int)position.y][(int)newX] == 0) position.x = newX;
    if (map[(int)newY][(int)position.x] == 0) position.y = newY;

    if (!canShoot) {
        shootTimer += dt;
        if (shootTimer >= SHOTGUN_COOLDOWN) {
            canShoot = true;
            shootTimer = 0.f;
        }
    }

    // Убрали проверку клика отсюда (она теперь в Game.cpp)

    if (flashTimer > 0.f) flashTimer -= dt;
}

void Player::Shoot() {
    ammo--;
    canShoot = false;
    flashTimer = 0.1f;
    std::cout << "BANG! Ammo left: " << ammo << std::endl;
}