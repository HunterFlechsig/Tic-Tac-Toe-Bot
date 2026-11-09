#include "Game.hpp"

#include <print>

Game::Game(Player& x, Player& o)
    : xPlayer(x), oPlayer(o) {
    xPlayer.setMarker(Marker::X);
    oPlayer.setMarker(Marker::O);
}

Result Game::run() {
    state = GameState{Marker::X};

    while (!state.isTerminal()) {
        // state.getBoard().printBoard();

        while (true) {
            const Player& player = currentPlayer();
            int move = player.getMove(state);
            // std::println("{} places at {}", static_cast<char>(player.getMarker()), move);

            if (!state.makeMove(move)) {
                // std::println("move failed getting another");
            } else {
                // std::println("move success");
                break;
            }
        }
    }

    auto winner = state.winner();
    // state.getBoard().printBoard();

    if (winner) {
        if(*winner == Marker::X) {
            return Result::Xwin;
        }
        return Result::Owin;
    } else {
        return Result::Tie;
    }
}

const Player& Game::currentPlayer() const {
    return state.getCurrentTurn() == Marker::X ? xPlayer : oPlayer;
}
