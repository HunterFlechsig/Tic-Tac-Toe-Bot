#pragma once

#include "GameState.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include <memory>
#include <array>

class Game {
    private:
        std::array<std::unique_ptr<Player>, 2> players {
            std::make_unique<MinimaxPlayer>(Marker::X, false, false),
            std::make_unique<RandomPlayer>(Marker::O)
        };
        GameState state{players[0]->getMarker()};

    public:
        void run();
        const Player& currentPlayer() const;
};
