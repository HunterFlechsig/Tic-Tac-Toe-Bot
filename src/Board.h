#pragma once

class Board {
    private:
        char board[9];
        int playedMoves = 0;
        void initializeBoard();

    public:
        Board();
        void printBoard();
        bool isBoardFull();
        bool makeMove(int index, char player);
        char getCell(int index) const;
};
