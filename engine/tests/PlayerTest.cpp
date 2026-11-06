#include <gtest/gtest.h>

#include "GameState.hpp"
#include "Marker.hpp"
#include "Player.hpp"

#include <vector>

TEST(PlayerTest, PlayerKeepsAssignedMarker) {
    RandomPlayer player(Marker::X);

    EXPECT_EQ(player.getMarker(), Marker::X);
}

TEST(PlayerTest, SeededRandomPlayerIsDeterministic) {
    GameState state(Marker::X);
    RandomPlayer first(Marker::X, 42);
    RandomPlayer second(Marker::X, 42);

    EXPECT_EQ(first.getMove(state), second.getMove(state));
}

TEST(PlayerTest, RandomPlayerReturnsNoMoveWhenNoneAreLegal) {
    GameState state(Marker::X);

    // X X X
    // O O .
    // . . .
    ASSERT_TRUE(state.makeMove(0));
    ASSERT_TRUE(state.makeMove(3));
    ASSERT_TRUE(state.makeMove(1));
    ASSERT_TRUE(state.makeMove(4));
    ASSERT_TRUE(state.makeMove(2));
    ASSERT_TRUE(state.legalMoves().empty());

    RandomPlayer player(Marker::O, 1);

    EXPECT_EQ(player.getMove(state), -1);
}

TEST(PlayerTest, RandomPlayerReturnsLegalMove) {
    GameState state(Marker::X);

    // X O X
    // O O X
    // . X .
    ASSERT_TRUE(state.makeMove(0));
    ASSERT_TRUE(state.makeMove(1));
    ASSERT_TRUE(state.makeMove(2));
    ASSERT_TRUE(state.makeMove(3));
    ASSERT_TRUE(state.makeMove(5));
    ASSERT_TRUE(state.makeMove(4));
    ASSERT_TRUE(state.makeMove(7));
    ASSERT_EQ(state.legalMoves(), (std::vector<int>{6, 8}));

    RandomPlayer player(Marker::O);
    const int move = player.getMove(state);

    EXPECT_TRUE(move == 6 || move == 8);
}

TEST(PlayerTest, MinimaxTakesImmediateWinningMove) {
    GameState state(Marker::X);

    // O . .
    // . . O
    // X X .
    ASSERT_TRUE(state.makeMove(6));
    ASSERT_TRUE(state.makeMove(0));
    ASSERT_TRUE(state.makeMove(7));
    ASSERT_TRUE(state.makeMove(5));

    MinimaxPlayer minimax(Marker::X, /*prune=*/false, /*useDepth=*/true);

    EXPECT_EQ(minimax.getMove(state), 8);
}

TEST(PlayerTest, MinimaxBlocksImmediateLoss) {
    GameState state(Marker::X);

    // . . .
    // O . .
    // X X .
    ASSERT_TRUE(state.makeMove(6));
    ASSERT_TRUE(state.makeMove(3));
    ASSERT_TRUE(state.makeMove(7));

    MinimaxPlayer minimax(Marker::O, /*prune=*/false, /*useDepth=*/true);

    EXPECT_EQ(minimax.getMove(state), 8);
}

TEST(PlayerTest, MinimaxPrefersImmediateWinOverBlocking) {
    GameState state(Marker::X);

    // . . .
    // O O .
    // X X .
    ASSERT_TRUE(state.makeMove(6));
    ASSERT_TRUE(state.makeMove(3));
    ASSERT_TRUE(state.makeMove(7));
    ASSERT_TRUE(state.makeMove(4));

    MinimaxPlayer minimax(Marker::X, /*prune=*/false, /*useDepth=*/true);

    EXPECT_EQ(minimax.getMove(state), 8);
}

TEST(PlayerTest, MinimaxChoosesMoveThatAvoidsForcedLoss) {
    GameState state(Marker::X);

    // X has taken a corner. The center is the only reply that draws.
    ASSERT_TRUE(state.makeMove(0));

    MinimaxPlayer minimax(Marker::O, /*prune=*/false, /*useDepth=*/true);

    EXPECT_EQ(minimax.getMove(state), 4);
}

TEST(PlayerTest, MinimaxPrefersFasterWin) {
    GameState state(Marker::X);

    // O . .
    // X X .
    // . . O
    // X wins at once by playing 5. Playing 1 also forces a win, one move later.
    ASSERT_TRUE(state.makeMove(3));
    ASSERT_TRUE(state.makeMove(0));
    ASSERT_TRUE(state.makeMove(4));
    ASSERT_TRUE(state.makeMove(8));

    MinimaxPlayer minimax(Marker::X, /*prune=*/false, /*useDepth=*/true);

    EXPECT_EQ(minimax.getMove(state), 5);
}

TEST(PlayerTest, AlphaBetaChoosesSameMoveAsMinimax) {
    GameState state(Marker::X);

    // O . .
    // X X .
    // . . O
    ASSERT_TRUE(state.makeMove(3));
    ASSERT_TRUE(state.makeMove(0));
    ASSERT_TRUE(state.makeMove(4));
    ASSERT_TRUE(state.makeMove(8));

    MinimaxPlayer minimax(Marker::X, /*prune=*/false, /*useDepth=*/true);
    MinimaxPlayer alphaBeta(Marker::X, /*prune=*/true, /*useDepth=*/true);

    EXPECT_EQ(alphaBeta.getMove(state), minimax.getMove(state));
}

TEST(PlayerTest, AlphaBetaSearchesNoMoreNodesThanMinimax) {
    GameState state(Marker::X);
    ASSERT_TRUE(state.makeMove(0));

    MinimaxPlayer minimax(Marker::O, /*prune=*/false, /*useDepth=*/true);
    MinimaxPlayer alphaBeta(Marker::O, /*prune=*/true, /*useDepth=*/true);

    minimax.getMove(state);
    alphaBeta.getMove(state);

    EXPECT_LE(alphaBeta.nodesExplored(), minimax.nodesExplored());
}

TEST(PlayerTest, MinimaxDoesNotLoseToRandomPlayer) {
    const unsigned seeds[] = {1, 2, 3, 4, 5};

    for (unsigned seed : seeds) {
        GameState state(Marker::X);
        MinimaxPlayer minimax(Marker::X, /*prune=*/true, /*useDepth=*/true);
        RandomPlayer random(Marker::O, seed);

        int movesPlayed = 0;
        while (!state.isTerminal()) {
            ASSERT_LT(movesPlayed, 9);
            const int move = state.getCurrentTurn() == Marker::X
                ? minimax.getMove(state)
                : random.getMove(state);
            ASSERT_TRUE(state.makeMove(move)) << "seed " << seed;
            ++movesPlayed;
        }

        if (state.winner().has_value()) {
            EXPECT_EQ(state.winner().value(), Marker::X) << "seed " << seed;
        }
    }

    for (unsigned seed : seeds) {
        GameState state(Marker::X);
        RandomPlayer random(Marker::X, seed);
        MinimaxPlayer minimax(Marker::O, /*prune=*/true, /*useDepth=*/true);

        int movesPlayed = 0;
        while (!state.isTerminal()) {
            ASSERT_LT(movesPlayed, 9);
            const int move = state.getCurrentTurn() == Marker::X
                ? random.getMove(state)
                : minimax.getMove(state);
            ASSERT_TRUE(state.makeMove(move)) << "seed " << seed;
            ++movesPlayed;
        }

        if (state.winner().has_value()) {
            EXPECT_EQ(state.winner().value(), Marker::O) << "seed " << seed;
        }
    }
}

TEST(PlayerTest, OptimalPlayersEndInDraw) {
    GameState state(Marker::X);
    MinimaxPlayer playerX(Marker::X, /*prune=*/false, /*useDepth=*/true);
    MinimaxPlayer playerO(Marker::O, /*prune=*/false, /*useDepth=*/true);

    int movesPlayed = 0;
    while (!state.isTerminal()) {
        ASSERT_LT(movesPlayed, 9);

        const int move = state.getCurrentTurn() == Marker::X
            ? playerX.getMove(state)
            : playerO.getMove(state);
        ASSERT_TRUE(state.makeMove(move));
        ++movesPlayed;
    }

    EXPECT_FALSE(state.winner().has_value());
    EXPECT_TRUE(state.getBoard().isFull());
}
