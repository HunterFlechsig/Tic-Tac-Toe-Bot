#include "Board.h"

#include <iostream>

void Board::initializeBoard() {
    for (int i = 0; i < 9; i++) {
        board[i] = ' ';
    }
}

Board::Board() {
    initializeBoard();
}

void Board::printBoard() {
    const char* horizontal = "+---+---+---+";
    std::cout << horizontal << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cout << "|";
        for (int j = 0; j < 3; j++) {
            std::cout << " " << board[i * 3 + j] << " |";
        }
        std::cout << std::endl;
        std::cout << horizontal << std::endl;
    }
}

bool Board::isBoardFull() {
    return playedMoves == 9;
}

bool Board::makeMove(int index, char player) {
    if (index < 0 || index >= 9) {
        return false;
    }
    if (board[index] != ' ') {
        return false;
    }
    board[index] = player;
    playedMoves++;
    return true;
}

char Board::getCell(int index) const {
    return board[index];
}
