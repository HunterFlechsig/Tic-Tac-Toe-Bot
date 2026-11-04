#include "Board.hpp"

#include <iostream>

Board::Board() {
    board.fill(Marker::Empty);
}

// create terminal print out for board
void Board::printBoard() const {
    const char* horizontal = "+---+---+---+";
    std::cout << horizontal << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cout << "|";
        for (int j = 0; j < 3; j++) {
            std::cout << " " << static_cast<char>(board[i * 3 + j]) << " |";
        }
        std::cout << std::endl;
        std::cout << horizontal << std::endl;
    }
}

// check if board is full
bool Board::isFull() const {
    for(Marker mark : board) {
        if(mark == Marker::Empty) {
            return false;
        }
    }
    return true;
}

// place marker if possible
bool Board::place(int index, Marker player) {
    if (index < 0 || index >= 9) {
        return false;
    }
    if (board[index] != Marker::Empty) {
        return false;
    }
    board[index] = player;
    return true;
}

// return mark at given index
Marker Board::getCell(int index) const {
    return board[index];
}
