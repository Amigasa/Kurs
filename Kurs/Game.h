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

    GameState currentState = GameState::Menu;

    // Тексты разделены на заголовки и подсказки
    sf::Text menuTitle;
    sf::Text menuHint;
    sf::Text pauseTitle;
    sf::Text pauseHint;

    sf::RectangleShape healthBarBg;
    sf::RectangleShape healthBarFill;

    sf::Sprite menuBgSprite;
    sf::Texture menuBgTexture;
    sf::RectangleShape darkOverlay;

    int menuSelection = 0;
    int pauseSelection = 0;
    int gameOverSelection = 0;
};