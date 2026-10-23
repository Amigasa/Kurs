#include "Game.h"
#include <windows.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Game game;
    game.Run();
    return 0;
}