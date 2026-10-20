#include "Game.h"
#include <iostream>
#include <cmath>

Game::Game() : window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "RootEvil - Raycaster") {
    window.setFramerateLimit(60);

    int tempMap[MAP_HEIGHT][MAP_WIDTH] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    for (int y = 0; y < MAP_HEIGHT; y++)
        for (int x = 0; x < MAP_WIDTH; x++)
            map[y][x] = tempMap[y][x];

    depthBuffer.resize(SCREEN_WIDTH);

    if (!font.loadFromFile("Resources/arial.ttf")) {
        std::cout << "Error loading font!" << std::endl;
    }
    hudText.setFont(font);
    hudText.setCharacterSize(24);
    hudText.setFillColor(sf::Color::White);
    hudText.setPosition(20.f, 20.f);
}

void Game::Run() {
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        player.Update(dt, map);

        window.clear(sf::Color(30, 30, 30));
        CastRays();
        DrawHUD();
        window.display();
    }
}

void Game::CastRays() {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float rayAngle = player.angle - FOV / 2.f + (float)x / SCREEN_WIDTH * FOV;
        float rayDirX = cos(rayAngle);
        float rayDirY = sin(rayAngle);

        float rayX = player.position.x;
        float rayY = player.position.y;
        float distance = 0.f;
        float step = 0.02f;
        bool hitWall = false;

        while (!hitWall && distance < 20.f) {
            rayX += rayDirX * step;
            rayY += rayDirY * step;
            distance += step;

            int mapX = (int)rayX;
            int mapY = (int)rayY;

            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) { hitWall = true; break; }
            if (map[mapY][mapX] == 1) hitWall = true;
        }

        distance *= cos(rayAngle - player.angle);
        depthBuffer[x] = distance;

        int wallHeight = (int)(SCREEN_HEIGHT / (distance + 0.001f));
        if (wallHeight > SCREEN_HEIGHT) wallHeight = SCREEN_HEIGHT;
        int drawStart = SCREEN_HEIGHT / 2 - wallHeight / 2;

        int shade = 255 - (int)(distance * 12.f);
        if (shade < 40) shade = 40;
        if (shade > 255) shade = 255;

        sf::RectangleShape line(sf::Vector2f(1.f, (float)wallHeight));
        line.setPosition((float)x, (float)drawStart);
        line.setFillColor(sf::Color(shade, 0, 0));
        window.draw(line);
    }
}

void Game::DrawHUD() {
    hudText.setString("HP: " + std::to_string(player.health) + "   AMMO: " + std::to_string(player.ammo));
    window.draw(hudText);
}