#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Constants.h"
#include "Player.h"
#include "Enemy.h"

class Game {
public:
    Game();
    void Run();

private:
    void CastRays();
    void DrawHUD();

    sf::RenderWindow window;
    Player player;
    int map[MAP_HEIGHT][MAP_WIDTH];
    std::vector<float> depthBuffer;
    std::vector<Enemy> enemies;

    sf::Font font;
    sf::Text hudText;
    sf::Clock clock;

    // Состояния игры
    GameState currentState = GameState::Menu;
    sf::Text menuText;
    sf::Text pauseText;

    // Здоровье
    sf::RectangleShape healthBarBg;
    sf::RectangleShape healthBarFill;
};