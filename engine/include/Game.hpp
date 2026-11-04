#pragma once

#include "Board.hpp"

#include <string>
#include <vector>

class Game {
    private:
        Board board;
        char currentPlayer;

        

        enum class Outcome { Xwin, Owin, Tie };

        static inline const std::string outcomeMessages[] = {
            "X wins!", "O wins!", "Tie!"
        };

        void printBoard();
        bool isGameOver();
        bool playMove(int index);
        int getPlayerMove();
        void switchPlayer();
        bool checkPlayerWin(char player);
        Outcome checkWinner();

    public:
        Game();
        std::vector<int> legalMoves() const;
        void run();
};
