#pragma once

#include "GameState.hpp"
#include "Player.hpp"
#include <vector>

enum Result {
    Xwin,
    Owin,
    Tie
};

class Game {
    Player& xPlayer;
    Player& oPlayer;
    GameState state{Marker::X};

public:
    Game(Player& xPlayer, Player& oPlayer);
    Result run();
    const Player& currentPlayer() const;
};
