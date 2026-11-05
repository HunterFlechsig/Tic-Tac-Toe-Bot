#include "Game.hpp"
#include <print>

void Game::run() {
    while(!state.isTerminal()) {
        state.getBoard().printBoard();

        while (true) {
            const Player& player = currentPlayer();
            int move = player.getMove(state);
            std::println("{} places at {}", static_cast<char>(player.getMarker()), move);

            if(!state.makeMove(move)) {
                std::println("move failed getting another");
            } else {
                std::println("move success");
                break;
            }
        }
    }

    auto winner = state.winner();
    state.getBoard().printBoard();

    if(winner) {
        std::println("{} Wins!", static_cast<char>(*winner));
    } else {
        std::println("Tie!");
    }
}

const Player& Game::currentPlayer() const {
    return state.getCurrentTurn() == Marker::X
        ? *players[0]
        : *players[1];
}