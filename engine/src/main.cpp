#include "Game.hpp"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    int i = 0;
    while (i < argc) {
        cout << "Argument " << i + 1 << ": " << argv[i]
             << endl;
        i++;
    }

    Game game{};
    game.run();
    return 0;
}
