#include "GameState.hpp"


GameState::GameState(Board& board, Marker current_mark) {
    this->board = board;
    this->currentTurn = current_mark;
}

void GameState::switchPlayer() {
    currentTurn = (currentTurn == Marker::X)
        ? Marker::X : Marker::O;
}

std::optional<GameState> GameState::afterMove(int index) const {
    GameState newState = *this;

    if(!newState.board.place(index, currentTurn)) {
        return std::nullopt;
    }

    newState.switchPlayer();
    return newState;
}

bool GameState::isTerminal(){
    return  board.isFull() || winner().has_value();
}

bool GameState::checkWinner(Marker mark) {
    static int winIndexes[8][3] = {
            {0, 1, 2},
            {3, 4, 5},
            {6, 7, 8},
            {0, 3, 6},
            {1, 4, 7},
            {2, 5, 8},
            {0, 4, 8},
            {2, 4, 6}
    };

    for (const auto& winLine : winIndexes) {
        if (board.getCell(winLine[0]) == mark &&
            board.getCell(winLine[1]) == mark &&
            board.getCell(winLine[2]) == mark) {
            return true;
        }
    }
    return false;
}

std::optional<Marker> GameState::winner() {
    if(checkWinner(Marker::X)) {
        return Marker::X;
    }

    if(checkWinner(Marker::O)) {
        return Marker::O;
    }

    return std::nullopt;
}


