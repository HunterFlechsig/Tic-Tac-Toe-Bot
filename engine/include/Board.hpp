#pragma once

#include <array>
#include "Marker.hpp"

class Board {
    private:
        std::array<Marker, 9> board;
        int playedMoves = 0;

    public:
        Board();
        void printBoard() const; 
        bool isFull() const; 
        bool place(int index, Marker marker);
        Marker getCell(int index) const;
};
