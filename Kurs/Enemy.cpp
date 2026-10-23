#include "Enemy.h"
#include "Constants.h"
#include <cmath>
#include <iostream>
#include <memory>

static std::shared_ptr<sf::Texture> sharedEnemyTexture = nullptr;

Enemy::Enemy(float startX, float startY) {
    position = { startX, startY };
    health = 50; // Умирает с 2 выстрелов (25 урона * 2 = 50)
    isAlive = true;

    if (!sharedEnemyTexture) {
        sharedEnemyTexture = std::make_shared<sf::Texture>();
        if (!sharedEnemyTexture->loadFromFile("Resources/enemy.png")) {
            std::cout << "FAILED TO LOAD enemy.png!" << std::endl;
            sf::Image img; img.create(100, 100, sf::Color::Red);
            sharedEnemyTexture->loadFromImage(img);
        }
    }
    sprite.setTexture(*sharedEnemyTexture);
}

void Enemy::Update(float dt, sf::Vector2f playerPos) {
    if (!isAlive) return;

    // Уменьшаем таймер покраснения
    if (hitTimer > 0.f) hitTimer -= dt;

    float dx = playerPos.x - position.x;
    float dy = playerPos.y - position.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 0.5f) {
        float speed = 1.5f;
        position.x += (dx / distance) * speed * dt;
        position.y += (dy / distance) * speed * dt;
    }
}

void Enemy::Draw(sf::RenderWindow& window, sf::Vector2f playerPos, float playerAngle, const std::vector<float>& depthBuffer) {
    if (!isAlive) return;

    float dx = position.x - playerPos.x;
    float dy = position.y - playerPos.y;
    float angleToEnemy = std::atan2(dy, dx);
    float angleDiff = angleToEnemy - playerAngle;

    while (angleDiff > 3.14159f) angleDiff -= 2.0f * 3.14159f;
    while (angleDiff < -3.14159f) angleDiff += 2.0f * 3.14159f;

    if (std::abs(angleDiff) < 3.14159f / 3.f) {
        float distance = std::sqrt(dx * dx + dy * dy);
        float distCorrected = distance * std::cos(angleDiff);
        if (distCorrected < 0.1f) distCorrected = 0.1f;

        int screenX = (int)((angleDiff / (3.14159f / 3.f) + 1.0f) * (SCREEN_WIDTH / 2.0f));

        if (screenX >= 0 && screenX < SCREEN_WIDTH) {
            float spriteHeight = SCREEN_HEIGHT / distCorrected;
            float scale = spriteHeight / 100.0f;
            sprite.setScale(scale, scale);

            float posX = screenX - (100.0f * scale) / 2.0f;
            float posY = (SCREEN_HEIGHT / 2.0f) - (100.0f * scale) / 2.0f;
            sprite.setPosition(sf::Vector2f(posX, posY));

            // Меняем цвет на красный, если монстр только что получил урон
            if (hitTimer > 0.f) {
                sprite.setColor(sf::Color::Red);
            }
            else {
                sprite.setColor(sf::Color::White);
            }

            if (distCorrected < depthBuffer[screenX]) {
                window.draw(sprite);
            }
        }
    }
}