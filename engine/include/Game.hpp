#pragma once

#include "GameState.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include <memory>
#include <array>

class Game {
    private:
        std::array<std::unique_ptr<Player>, 2> players {
            std::make_unique<MinimaxPlayer>(Marker::X, true, true),
            std::make_unique<HumanPlayer>(Marker::O)
        };
        GameState state{players[0]->getMarker()};

    public:
        void run();
        const Player& currentPlayer() const;
};
