#pragma once

#include "GameState.hpp"

#include <cstdint>
#include <random>

class Player {
    protected:
        Marker marker;
        static const int inf = 10000000;
        static const int neg_inf = -10000000;

    public:
        explicit Player(Marker marker) : marker(marker) {}

        virtual ~Player() = default; // auto destroy unique_ptr

        Marker getMarker() const {
            return marker;
        }

        virtual int getMove(const GameState& state) const = 0;
};

class HumanPlayer : public Player {
public:
    explicit HumanPlayer(Marker marker)
        : Player(marker) {}

    int getMove(const GameState& state) const override;

private:
    int getInput(const GameState& state);
};

class RandomPlayer : public Player {
public:
    explicit RandomPlayer(Marker marker);
    explicit RandomPlayer(Marker marker, std::uint32_t seed);

    int getMove(const GameState& state) const override;

private:
    int getInput(const GameState& state);
    mutable std::mt19937 generator;
};

class MinimaxPlayer : public Player {
public:
    bool prune;
    bool useDepth;

    explicit MinimaxPlayer(Marker marker, bool prune, bool useDepth)
        : Player(marker), prune(prune), useDepth(useDepth) {}

    int getMove(const GameState& state) const override;
    int nodesExplored() const { return nodesExplored_; }

private:
    int bestMove(const GameState& state) const;
    int minimax(const GameState& state, int alpha, int beta, int& nodes, bool prune,  int depth) const;

    mutable int nodesExplored_ = 0;
};