#include "GameState.hpp"


GameState::GameState(Marker current_mark) {
    this->currentTurn = current_mark;
}

void GameState::switchPlayer() {
    currentTurn = (currentTurn == Marker::X)
        ? Marker::O : Marker::X;
}

bool GameState::makeMove(int index) {
    if(!board.place(index, currentTurn)) {
        return false;
    }

    switchPlayer();
    return true;
}

bool GameState::isTerminal() const {
    return  board.isFull() || winner().has_value();
}

bool GameState::checkWinner(Marker mark) const {
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

std::optional<Marker> GameState::winner() const {
    if(checkWinner(Marker::X)) {
        return Marker::X;
    }

    if(checkWinner(Marker::O)) {
        return Marker::O;
    }

    return std::nullopt;
}

Marker GameState::getCurrentTurn() const {
    return currentTurn;
}

std::vector<int> GameState::legalMoves() const {
    std::vector<int> moves;

    for(int i = 0; i < 9; i++) {
        if(board.getCell(i) == Marker::Empty) {
            moves.push_back(i);
        }
    }
    return moves;
}

const Board& GameState::getBoard() const {
    return board;
}