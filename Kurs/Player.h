#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

class Player {
public:
    Player();
    void Update(float dt, int map[MAP_HEIGHT][MAP_WIDTH]);

    sf::Vector2f position;
    float angle;
    int health;
    int ammo;
};
