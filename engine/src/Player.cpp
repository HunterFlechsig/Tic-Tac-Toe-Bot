#include "Player.hpp"
#include <random>

int Player::getMove() const {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distr(-1, 10);
    return distr(gen);
}