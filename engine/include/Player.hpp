#pragma once

#include "GameState.hpp"

#include <cstdint>
#include <random>

class Player {
    protected:
        Marker marker = Marker::Empty;
        static const int inf = 10000000;
        static const int neg_inf = -10000000;

    public:
        Player() = default;

        virtual ~Player() = default; // auto destroy unique_ptr

        void setMarker(Marker next) {
            marker = next;
        }

        Marker getMarker() const {
            return marker;
        }

        virtual int getMove(const GameState& state) const = 0;
};

class HumanPlayer : public Player {
public:
    HumanPlayer() = default;

    int getMove(const GameState& state) const override;

private:
    int getInput(const GameState& state);
};

class RandomPlayer : public Player {
public:
    RandomPlayer();
    explicit RandomPlayer(std::uint32_t seed);

    int getMove(const GameState& state) const override;

private:
    int getInput(const GameState& state);
    mutable std::mt19937 generator;
};

class MinimaxPlayer : public Player {
public:
    bool prune;
    bool useDepth;

    explicit MinimaxPlayer(bool prune, bool useDepth)
        : prune(prune), useDepth(useDepth) {}

    int getMove(const GameState& state) const override;
    std::vector<int> nodesExplored() const { return nodesExplored_; }
    std::vector<long long> decisionTime() const { return decisionNanos_; }

private:
    int bestMove(const GameState& state) const;
    int minimax(const GameState& state, int alpha, int beta, int& nodes, bool prune,  int depth) const;

    mutable std::vector<int> nodesExplored_;
    mutable  std::vector<long long> decisionNanos_;
};