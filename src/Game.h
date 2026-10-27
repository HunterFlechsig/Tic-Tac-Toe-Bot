#pragma once

#include "Board.h"

#include <string>

class Game {
    private:
        Board board;
        char currentPlayer;

        static constexpr int winIndexes[8][3] = {
            {0, 1, 2},
            {3, 4, 5},
            {6, 7, 8},
            {0, 3, 6},
            {1, 4, 7},
            {2, 5, 8},
            {0, 4, 8},
            {2, 4, 6}
        };

        enum class Outcome { Xwin, Ywin, Tie };

        static inline const std::string outcomeMessages[] = {
            "X wins!", "Y wins!", "Tie!"
        };

        void printBoard();
        bool isGameOver();
        bool playMove(int index);
        int getPlayerMove();
        void switchPlayer();
        bool checkPlayerWin(char player);
        std::string checkWinner();

    public:
        Game();
        void run();
};
