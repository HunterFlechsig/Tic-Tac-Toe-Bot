#include "Game.h"

#include <exception>
#include <iostream>
#include <string>

namespace {

    void numberToRowCol(int number, int& row, int& column) {
        constexpr int columnsPerRow = 3;
        row = number / columnsPerRow;
        column = number % columnsPerRow;
    }

}

Game::Game() {
    currentPlayer = 'X';
}

void Game::printBoard() {
    board.printBoard();
}

bool Game::isGameOver() {
    return board.isBoardFull();
}

bool Game::playMove(int index) {
    try {
        int row;
        int column;
        numberToRowCol(index, row, column);
        board.makeMove(row, column, currentPlayer);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    return true;
}

void Game::switchPlayer() {
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
}

bool Game::checkPlayerWin(char player) {
    for (int line = 0; line < 8; line++) {
        bool won = true;
        for (int cell = 0; cell < 3; cell++) {
            int index = winIndexes[line][cell];
            int row;
            int column;
            numberToRowCol(index, row, column);
            if (board.getCell(row, column) != player) {
                won = false;
                break;
            }
        }
        if (won) {
            return true;
        }
    }
    return false;
}

std::string Game::checkWinner() {
    bool x = checkPlayerWin('X');
    bool o = checkPlayerWin('O');

    Outcome out = Outcome::Ywin;

    if(!x && !o) {
        out = Outcome::Tie;
    }

    if(x) {
        out = Outcome::Xwin;
    }

    return outcomeMessages[static_cast<int>(out)];    
}

int Game::getPlayerMove() {
    int index;

    std::cout << "Enter position [0-8]: ";
    std::cin >> index;

    return index;
}


void Game::run() {
    while(!isGameOver()) {
        while(true) {
            printBoard();
            int index = getPlayerMove();
            bool allowed = playMove(index);
            if(allowed) break;
        }
        if(checkPlayerWin(currentPlayer)) {
            break;
        }
        switchPlayer();
    }
    printBoard();
    std::cout << checkWinner() << std::endl;
}