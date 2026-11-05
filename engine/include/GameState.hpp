#pragma once

#include "Board.hpp"
#include "Marker.hpp"
#include <optional>
#include <vector>

class GameState {
    private:       
        Marker currentTurn = Marker::X;
        Board board;
        bool checkWinner(Marker mark) const;

    public: 
        GameState(Marker current_mark);
        bool makeMove(int index);
        void switchPlayer();
        bool isTerminal() const;
        Marker getCurrentTurn() const;
        std::optional<Marker> winner() const;     
        const Board& getBoard() const;
        std::vector<int> legalMoves() const;
};