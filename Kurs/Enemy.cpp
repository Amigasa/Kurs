#include "Enemy.h"
#include "Constants.h"
#include <cmath>
#include <iostream>

// Инициализация статической текстуры
sf::Texture Enemy::texture;

Enemy::Enemy(float startX, float startY) {
    position = { startX, startY };
    health = 50;
    isAlive = true;

    // Загружаем текстуру (сработает только 1 раз)
    if (texture.getSize().x == 0) {
        if (!texture.loadFromFile("Resources/enemy.png")) {
            std::cout << "Error loading enemy.png!" << std::endl;
        }
    }
    sprite.setTexture(texture);
}

void Enemy::Update(float dt, sf::Vector2f playerPos) {
    if (!isAlive) return;

    float dx = playerPos.x - position.x;
    float dy = playerPos.y - position.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 0.5f) {
        float speed = 1.0f;
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

    // Нормализация угла
    while (angleDiff > 3.14159f) angleDiff -= 2.0f * 3.14159f;
    while (angleDiff < -3.14159f) angleDiff += 2.0f * 3.14159f;

    // Если монстр в поле зрения
    if (std::abs(angleDiff) < 3.14159f / 3.f) {
        float distance = std::sqrt(dx * dx + dy * dy);
        float distCorrected = distance * std::cos(angleDiff);

        // Защита от деления на ноль (если монстр прямо в игроке)
        if (distCorrected < 0.1f) distCorrected = 0.1f;

        int screenX = (int)((angleDiff / (3.14159f / 3.f) + 1.0f) * (SCREEN_WIDTH / 2.0f));

        // ЖЕЛЕЗОБЕТОННАЯ ЗАЩИТА ОТ ВЫЛЕТА:
        if (screenX >= 0 && screenX < SCREEN_WIDTH) {
            float spriteHeight = SCREEN_HEIGHT / distCorrected;
            float scale = spriteHeight / texture.getSize().y;
            sprite.setScale(scale, scale);

            float posX = screenX - (texture.getSize().x * scale) / 2.0f;
            float posY = SCREEN_HEIGHT / 2.0f - spriteHeight / 2.0f;
            sprite.setPosition(sf::Vector2f(posX, posY));

            // Отрисовка только если монстр ближе, чем стена
            if (distCorrected < depthBuffer[screenX]) {
                window.draw(sprite);
            }
        }
    }
}