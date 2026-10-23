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

    if (!font.loadFromFile("Resources/arial.ttf")) std::cout << "Error font!" << std::endl;

    hudText.setFont(font);
    hudText.setCharacterSize(24);
    hudText.setFillColor(sf::Color::White);
    hudText.setPosition(20.f, 20.f);

    // Настройка фона меню
    if (!menuBgTexture.loadFromFile("Resources/menu_bg.png")) {
        std::cout << "Ошибка загрузки фона меню!" << std::endl;
    }
    menuBgSprite.setTexture(menuBgTexture);
    sf::Vector2u bgSize = menuBgTexture.getSize();
    if (bgSize.x > 0 && bgSize.y > 0) {
        menuBgSprite.setScale((float)SCREEN_WIDTH / bgSize.x, (float)SCREEN_HEIGHT / bgSize.y);
    }

    darkOverlay.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    darkOverlay.setFillColor(sf::Color(0, 0, 0, 150));

    // Функция центрирования текста
    auto centerText = [](sf::Text& text, float yOffset) {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
        text.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + yOffset);
        };

    // Главное меню
    menuTitle.setFont(font);
    menuTitle.setCharacterSize(90);
    menuTitle.setFillColor(sf::Color::Red);
    menuTitle.setStyle(sf::Text::Bold);
    menuTitle.setString("ROOT EVIL");
    centerText(menuTitle, -180.f);

    menuHint.setFont(font);
    menuHint.setCharacterSize(36);
    menuHint.setFillColor(sf::Color::White);
    menuHint.setString("START GAME\nEXIT");
    centerText(menuHint, 20.f);

    // Пауза
    pauseTitle.setFont(font);
    pauseTitle.setCharacterSize(80);
    pauseTitle.setFillColor(sf::Color::White);
    pauseTitle.setStyle(sf::Text::Bold);
    pauseTitle.setString("PAUSED");
    centerText(pauseTitle, -80.f);

    pauseHint.setFont(font);
    pauseHint.setCharacterSize(30);
    pauseHint.setFillColor(sf::Color::Yellow);
    pauseHint.setString("RESUME");
    centerText(pauseHint, 20.f);

    // Полоска здоровья
    healthBarBg.setSize(sf::Vector2f(200.f, 20.f));
    healthBarBg.setPosition(20.f, 50.f);
    healthBarBg.setFillColor(sf::Color(50, 50, 50));

    healthBarFill.setSize(sf::Vector2f(200.f, 20.f));
    healthBarFill.setPosition(20.f, 50.f);
    healthBarFill.setFillColor(sf::Color::Red);

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

            // --- ЛОГИКА МЕНЮ ---
            if (currentState == GameState::Menu) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
                        menuSelection--;
                        if (menuSelection < 0) menuSelection = 1;
                    }
                    if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
                        menuSelection++;
                        if (menuSelection > 1) menuSelection = 0;
                    }
                    if (event.key.code == sf::Keyboard::Enter) {
                        if (menuSelection == 0) {
                            currentState = GameState::Playing;
                            sf::Mouse::setPosition(sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), window);
                        }
                        else {
                            window.close();
                        }
                    }
                }
            }
            // --- ЛОГИКА ПАУЗЫ ---
            else if (currentState == GameState::Paused) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
                        pauseSelection--;
                        if (pauseSelection < 0) pauseSelection = 1;
                    }
                    if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
                        pauseSelection++;
                        if (pauseSelection > 1) pauseSelection = 0;
                    }
                    if (event.key.code == sf::Keyboard::Enter) {
                        if (pauseSelection == 0) {
                            currentState = GameState::Playing;
                            window.setMouseCursorVisible(false);
                            sf::Mouse::setPosition(sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), window);
                        }
                        else {
                            currentState = GameState::Menu;
                            window.setMouseCursorVisible(true);
                        }
                    }
                    if (event.key.code == sf::Keyboard::Escape) {
                        currentState = GameState::Playing;
                        window.setMouseCursorVisible(false);
                        sf::Mouse::setPosition(sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), window);
                    }
                }
            }
            // --- ЛОГИКА СМЕРТИ ---
            else if (currentState == GameState::GameOver) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down ||
                        event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::S) {
                        gameOverSelection = 1 - gameOverSelection; // Переключаем 0 и 1
                    }
                    if (event.key.code == sf::Keyboard::Enter) {
                        if (gameOverSelection == 0) {
                            player = Player();
                            for (auto& e : enemies) e.isAlive = true;
                            currentState = GameState::Playing;
                            window.setMouseCursorVisible(false);
                            sf::Mouse::setPosition(sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), window);
                        }
                        else {
                            currentState = GameState::Menu;
                            window.setMouseCursorVisible(true);
                            menuSelection = 0;
                        }
                    }
                }
            }
            // --- ЛОГИКА ИГРЫ ---
            else if (currentState == GameState::Playing) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    currentState = GameState::Paused;
                    window.setMouseCursorVisible(true);
                    pauseSelection = 0;
                }

                if (event.type == sf::Event::MouseMoved) {
                    float mouseDeltaX = event.mouseMove.x - (SCREEN_WIDTH / 2.0f);
                    player.angle += mouseDeltaX * 0.002f;
                    sf::Mouse::setPosition(sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), window);
                }

                // СТРЕЛЬБА
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (player.canShoot && player.ammo > 0) {
                        player.Shoot();

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
                                enemy.hitTimer = 0.1f;

                                if (enemy.health <= 0) {
                                    enemy.isAlive = false;
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(30, 30, 30));

        // --- ОТРИСОВКА ---
        if (currentState == GameState::Menu) {
            window.draw(menuBgSprite);
            window.draw(darkOverlay);
            window.draw(menuTitle);

            if (menuSelection == 0) {
                menuHint.setString("> START GAME <\n   EXIT   ");
                menuHint.setFillColor(sf::Color::Yellow);
            }
            else {
                menuHint.setString("   START GAME   \n> EXIT <");
                menuHint.setFillColor(sf::Color::White);
            }
            sf::FloatRect bounds = menuHint.getLocalBounds();
            menuHint.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
            menuHint.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 20.f);
            window.draw(menuHint);
        }
        else if (currentState == GameState::Playing) {
            player.Update(dt, map);

            for (auto& enemy : enemies) {
                enemy.Update(dt, player.position);

                float dx = enemy.position.x - player.position.x;
                float dy = enemy.position.y - player.position.y;
                float dist = std::sqrt(dx * dx + dy * dy);

                if (dist < 0.6f && enemy.isAlive) {
                    player.health -= 0.5f;
                }
            }

            if (player.health <= 0) {
                player.health = 0;
                currentState = GameState::GameOver;
                window.setMouseCursorVisible(true);
                gameOverSelection = 0;
            }

            CastRays();
            for (auto& enemy : enemies) enemy.Draw(window, player.position, player.angle, depthBuffer);

            window.draw(player.weaponSprite);
            if (player.flashTimer > 0.f) window.draw(player.flashSprite);

            DrawHUD();
        }
        else if (currentState == GameState::Paused) {
            CastRays();
            darkOverlay.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(darkOverlay);
            window.draw(pauseTitle);

            if (pauseSelection == 0) {
                pauseHint.setString("> RESUME <\n   EXIT   ");
                pauseHint.setFillColor(sf::Color::Yellow);
            }
            else {
                pauseHint.setString("   RESUME   \n> EXIT <");
                pauseHint.setFillColor(sf::Color::White);
            }
            sf::FloatRect b = pauseHint.getLocalBounds();
            pauseHint.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
            pauseHint.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 20.f);
            window.draw(pauseHint);
        }
        else if (currentState == GameState::GameOver) {
            window.draw(menuBgSprite);
            darkOverlay.setFillColor(sf::Color(50, 0, 0, 200));
            window.draw(darkOverlay);

            menuTitle.setString("YOU DIED");

            if (gameOverSelection == 0) {
                menuHint.setString("> RESTART <\n  MAIN MENU  ");
                menuHint.setFillColor(sf::Color::Yellow);
            }
            else {
                menuHint.setString("  RESTART  \n> MAIN MENU <");
                menuHint.setFillColor(sf::Color::White);
            }
            sf::FloatRect bounds = menuHint.getLocalBounds();
            menuHint.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
            menuHint.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 20.f);

            window.draw(menuTitle);
            window.draw(menuHint);
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