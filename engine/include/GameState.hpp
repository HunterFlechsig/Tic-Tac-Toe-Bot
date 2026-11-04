#pragma once

#include "Board.hpp"
#include "Marker.hpp"
#include <optional>

class GameState {
    private:       
        Board board;
        Marker currentTurn = Marker::X;
        bool checkWinner(Marker mark);

        static constexpr int winIndexes[8][3] = {
            {0, 1, 2},
            {3, 4, 5},
            {6, 7, 8},
            {0, 3, 6},
            {1, 4, 7},
            {2, 5, 8},
            {0, 4, 8},
            {2, 4, 6}
        };

    public: 
        GameState(Board& board, Marker current_mark);
        std::optional<GameState> afterMove(int index) const;
        void switchPlayer();
        bool isTerminal();
        std::optional<Marker> winner();
};