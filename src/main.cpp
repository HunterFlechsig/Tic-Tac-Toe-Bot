#include "Board.h"
#include "Game.h"

#include <iostream>


int main() {
    std::cout << "Hello from tic-tac-toe-bot.\n";

    Game game{};
    game.run();

    return 0;
}
