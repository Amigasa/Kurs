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

    // Шрифт
    if (!font.loadFromFile("Resources/arial.ttf")) std::cout << "Error font!" << std::endl;

    hudText.setFont(font);
    hudText.setCharacterSize(24);
    hudText.setFillColor(sf::Color::White);
    hudText.setPosition(20.f, 20.f);

    // Текст меню
    menuText.setFont(font);
    menuText.setCharacterSize(48);
    menuText.setFillColor(sf::Color::Yellow);
    menuText.setString("ROOT EVIL\n\nPress ENTER to Start\nPress ESC to Exit");
    menuText.setPosition(SCREEN_WIDTH / 2.f - 250.f, SCREEN_HEIGHT / 2.f - 100.f);

    pauseText.setFont(font);
    pauseText.setCharacterSize(48);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setString("PAUSED\n\nPress ENTER to Resume");
    pauseText.setPosition(SCREEN_WIDTH / 2.f - 150.f, SCREEN_HEIGHT / 2.f - 50.f);

    // Полоска здоровья
    healthBarBg.setSize(sf::Vector2f(200.f, 20.f));
    healthBarBg.setPosition(20.f, 50.f);
    healthBarBg.setFillColor(sf::Color(50, 50, 50));

    healthBarFill.setSize(sf::Vector2f(200.f, 20.f));
    healthBarFill.setPosition(20.f, 50.f);
    healthBarFill.setFillColor(sf::Color::Red);

    // Создание монстров
    enemies.push_back(Enemy(5.5f, 5.5f));
    enemies.push_back(Enemy(10.5f, 10.5f));
    enemies.push_back(Enemy(5.5f, 10.5f));
}

void Game::Run() {
    window.setMouseCursorVisible(false);
    sf::Mouse::setPosition(sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), window);

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (currentState == GameState::Menu) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Enter) {
                        currentState = GameState::Playing;
                        sf::Mouse::setPosition(sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), window);
                    }
                    if (event.key.code == sf::Keyboard::Escape) window.close();
                }
            }
            else if (currentState == GameState::Paused) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    currentState = GameState::Playing;
                    window.setMouseCursorVisible(false);
                    sf::Mouse::setPosition(sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), window);
                }
            }
            else if (currentState == GameState::Playing) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    currentState = GameState::Paused;
                    window.setMouseCursorVisible(true);
                }

                if (event.type == sf::Event::MouseMoved) {
                    float mouseDeltaX = event.mouseMove.x - (SCREEN_WIDTH / 2.0f);
                    player.angle += mouseDeltaX * 0.002f;
                    sf::Mouse::setPosition(sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), window);
                }
            }
        }

        window.clear(sf::Color(30, 30, 30));

        if (currentState == GameState::Menu) {
            window.draw(menuText);
        }
        else if (currentState == GameState::Playing) {
            player.Update(dt, map);

            // Обновление врагов и проверка урона
            for (auto& enemy : enemies) {
                enemy.Update(dt, player.position);

                float dx = enemy.position.x - player.position.x;
                float dy = enemy.position.y - player.position.y;
                float dist = std::sqrt(dx * dx + dy * dy);

                if (dist < 0.6f && enemy.isAlive) {
                    player.health -= 0.5f; // Урон каждую долю секунды
                }
            }

            if (player.health <= 0) {
                player.health = 0;
                currentState = GameState::GameOver;
            }

            // Проверка попадания (ЛКМ)
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && player.canShoot && player.ammo > 0) {
                for (auto& enemy : enemies) {
                    if (!enemy.isAlive) continue;
                    float dx = enemy.position.x - player.position.x;
                    float dy = enemy.position.y - player.position.y;
                    float angleToEnemy = std::atan2(dy, dx);
                    float angleDiff = angleToEnemy - player.angle;

                    while (angleDiff > 3.14159f) angleDiff -= 2.0f * 3.14159f;
                    while (angleDiff < -3.14159f) angleDiff += 2.0f * 3.14159f;

                    if (std::abs(angleDiff) < 0.15f) {
                        enemy.health -= SHOTGUN_DAMAGE;
                        if (enemy.health <= 0) enemy.isAlive = false;
                        break;
                    }
                }
            }

            CastRays();
            for (auto& enemy : enemies) enemy.Draw(window, player.position, player.angle, depthBuffer);

            window.draw(player.weaponSprite);
            if (player.flashTimer > 0.f) window.draw(player.flashSprite);

            DrawHUD();
        }
        else if (currentState == GameState::Paused) {
            CastRays();
            window.draw(pauseText);
        }
        else if (currentState == GameState::GameOver) {
            menuText.setString("YOU DIED\n\nPress ENTER to Restart");
            window.draw(menuText);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                // Рестарт игры
                player = Player();
                for (auto& e : enemies) e.isAlive = true;
                currentState = GameState::Playing;
            }
        }

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
        if (distance < 0.1f) distance = 0.1f;

        depthBuffer[x] = distance;

        int wallHeight = (int)(SCREEN_HEIGHT / distance);
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
    float healthPercent = player.health / MAX_HEALTH;
    if (healthPercent < 0) healthPercent = 0;
    healthBarFill.setSize(sf::Vector2f(200.f * healthPercent, 20.f));

    hudText.setString("HP: " + std::to_string((int)player.health) + "   AMMO: " + std::to_string(player.ammo));
    window.draw(hudText);
    window.draw(healthBarBg);
    window.draw(healthBarFill);
}