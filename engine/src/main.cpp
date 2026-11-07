#include "Game.hpp"
#include "Player.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    int i = 0;
    while (i < argc) {
        std::cout << "Argument " << i + 1 << ": " << argv[i] << std::endl;
        i++;
    }

    MinimaxPlayer x(true, true);
    HumanPlayer o;
    Game game(x, o);
    game.run();
    return 0;
}
