#include "Board.h"

#include <iostream>

void Board::initializeBoard() {
    for (int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
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
            std::cout << " " << board[i][j] << " |";
        }
        std::cout << std::endl;
        std::cout << horizontal << std::endl;
    }
}

bool Board::isBoardFull() {
    return playedMoves == 9;
}

void Board::makeMove(int row, int column, char player) {
    if (row < 0 || row >= 3 || column < 0 || column >= 3) {
        std::cout << "Invalid move. Please try again." << std::endl;
        return;
    }
    if (board[row][column] != ' ') {
        std::cout << "Cell already occupied. Please try again." << std::endl;
        return;
    }
    board[row][column] = player;
    playedMoves++;
}

char Board::getCell(int row, int column) const {
    return board[row][column];
}
