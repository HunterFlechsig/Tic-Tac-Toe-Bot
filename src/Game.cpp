#include "Game.h"

#include <iostream>
#include <limits>
#include <vector>


Game::Game() {
    currentPlayer = 'X';
}

void Game::printBoard() {
    board.printBoard();
}

bool Game::isGameOver() {
    return board.isBoardFull() || checkPlayerWin('X') || checkPlayerWin('O');
}

bool Game::playMove(int index) {
    return board.makeMove(index, currentPlayer);
}

std::vector<int> Game::legalMoves() const {
    std::vector<int> moves;
    for (int index = 0; index < 9; index++) {
        if (board.getCell(index) == ' ') {
            moves.push_back(index);
        }
    }
    return moves;
}

void Game::switchPlayer() {
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
}

bool Game::checkPlayerWin(char player) {
    for (int[3] winLine : winLines) {
        if (board.getCell(winLine[0]) == player &&
            board.getCell(winLine[1]) == player &&
            board.getCell(winLine[2]) == player) {
            return true;
        }
    }
    return false;
}

Game::Outcome Game::checkWinner() {
    if (checkPlayerWin('X')) {
        return Outcome::Xwin;
    }
    if (checkPlayerWin('O')) {
        return Outcome::Owin;
    }
    return Outcome::Tie;
}

int Game::getPlayerMove() {
    while (true) {
        std::cout << "Enter position [0-8]: ";
        int index;
        if (!(std::cin >> index)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid move. Please try again." << std::endl;
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (index < 0 || index > 8) {
            std::cout << "Invalid move. Please try again." << std::endl;
            continue;
        }
        return index;
    }
}

void Game::run() {
    while (!isGameOver()) {
        printBoard();
        int index = getPlayerMove();
        if (!playMove(index)) {
            std::cout << "Invalid move. Please try again." << std::endl;
            continue;
        }
        switchPlayer();
    }
    printBoard();
    std::cout << outcomeMessages[static_cast<int>(checkWinner())] << std::endl;
}