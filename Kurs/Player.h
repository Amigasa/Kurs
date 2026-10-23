#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

class Player {
public:
    Player();
    void Update(float dt, int map[MAP_HEIGHT][MAP_WIDTH]);
    void Shoot();

    sf::Vector2f position;
    float angle;
    float health;
    int ammo;

    sf::Sprite weaponSprite;
    sf::Texture weaponTexture;
    bool canShoot = true;
    float shootTimer = 0.f;

    sf::Sprite flashSprite;
    sf::Texture flashTexture;
    float flashTimer = 0.f;
};