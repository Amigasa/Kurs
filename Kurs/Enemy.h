#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Enemy {
public:
    Enemy(float startX, float startY);
    void Update(float dt, sf::Vector2f playerPos);
    void Draw(sf::RenderWindow& window, sf::Vector2f playerPos, float playerAngle, const std::vector<float>& depthBuffer);

    sf::Vector2f position;
    int health;
    bool isAlive;
    sf::Sprite sprite;

    float hitTimer = 0.f;
};