#include "Player.hpp"
#include <random>
#include <iostream>
#include <limits>
#include <algorithm>
#include <print>

RandomPlayer::RandomPlayer(Marker marker)
    : Player(marker), generator(std::random_device{}()) {}

RandomPlayer::RandomPlayer(Marker marker, std::uint32_t seed)
    : Player(marker), generator(seed) {}

int RandomPlayer::getMove(const GameState& state) const {
    auto legal_moves = state.legalMoves();
    if (legal_moves.empty()) {
        return -1;
    }

    std::uniform_int_distribution<int> distr(
        0, static_cast<int>(legal_moves.size()) - 1);
    return legal_moves[distr(generator)];
}

int HumanPlayer::getMove(const GameState& state) const {
    int index;
    bool validInput = false;

    while(!validInput) {
        std::cout << "What move? ";
        std::cin >> index;

        if (std::cin.fail()) {
            std::cout << "Invalid input! Expected an integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }
        else {
            validInput = true;
        }
    }

    return index;
}

int MinimaxPlayer::getMove(const GameState& state) const {
    return bestMove(state);
}

int MinimaxPlayer::bestMove(const GameState& state) const {
    int bestScore = neg_inf;
    int bestMove = -1;
    int nodes_explored = 0;

    auto legal_moves = state.legalMoves();
    for(auto move : legal_moves) {
        GameState childState = state;
        childState.makeMove(move);
        nodes_explored++;

        int score = minimax(childState, neg_inf, inf, nodes_explored, prune, 0);

        if(score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    nodesExplored_ = nodes_explored;
    std::println("Nodes explored {}", nodes_explored);
    return bestMove;
} 

int MinimaxPlayer::minimax(const GameState& state, int alpha, int beta, int& nodes, bool prune, int depth) const {    
    if(state.isTerminal()) {
        auto winner = state.winner();

        if(!winner) {
            return 0;
        }

        if(*winner == marker) {
            return 10 - depth;
        }

        return depth - 10;
    }

    auto legal_moves = state.legalMoves();

    if (state.getCurrentTurn() == marker) {
        int best = neg_inf;

        for(auto move : legal_moves) {
            nodes++;
            GameState childState = state;
            childState.makeMove(move);
            int score = minimax(childState, alpha, beta, nodes, prune, 
                useDepth ? depth+1 : depth);
            best = std::max(score, best);

            if(prune) {
                alpha = std::max(alpha, best);
                if (beta <= alpha) {
                    break;
                }
            }
        }
    
        return best;             
    } else {
        int best = inf;
        for(auto move : legal_moves) {
            nodes++;
            GameState childState = state;
            childState.makeMove(move);

            int score = minimax(childState, alpha, beta, nodes, prune, 
                useDepth ? depth+1 : depth);
            best = std::min(score, best);

            if(prune) {
                beta = std::min(beta, best);
                if (beta <= alpha) {
                    break;
                }
            }
        }
    
        return best;  
    }
}