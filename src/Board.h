#pragma once

class Board {
    private:
        char board[3][3];
        int playedMoves = 0;
        void initializeBoard();

    public:
        Board();
        void printBoard();
        bool isBoardFull();
        bool makeMove(int row, int column, char player);
        char getCell(int row, int column) const;
};
