#include <gtest/gtest.h>

#include "GameState.hpp"
#include "Marker.hpp"

#include <optional>
#include <vector>

TEST(GameStateTest, NewStateStartsWithExpectedTurn) {
    GameState state(Marker::O);

    EXPECT_EQ(state.getCurrentTurn(), Marker::O);
}

TEST(GameStateTest, NewStateHasNineLegalMoves) {
    GameState state(Marker::X);

    EXPECT_EQ(state.legalMoves(), (std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8}));
}

TEST(GameStateTest, NewStateIsNotTerminal) {
    GameState state(Marker::X);

    EXPECT_FALSE(state.isTerminal());
    EXPECT_FALSE(state.winner().has_value());
}

TEST(GameStateTest, SuccessfulMoveUpdatesBoard) {
    GameState state(Marker::X);

    EXPECT_TRUE(state.makeMove(4));
    EXPECT_EQ(state.getBoard().getCell(4), Marker::X);
}

TEST(GameStateTest, SuccessfulMoveChangesTurn) {
    GameState state(Marker::X);

    EXPECT_TRUE(state.makeMove(0));
    EXPECT_EQ(state.getCurrentTurn(), Marker::O);
}

TEST(GameStateTest, FailedMoveDoesNotChangeTurn) {
    GameState state(Marker::X);

    ASSERT_TRUE(state.makeMove(0));
    EXPECT_EQ(state.getCurrentTurn(), Marker::O);

    EXPECT_FALSE(state.makeMove(0));
    EXPECT_EQ(state.getCurrentTurn(), Marker::O);
}

TEST(GameStateTest, FailedMoveDoesNotChangeBoard) {
    GameState state(Marker::X);

    ASSERT_TRUE(state.makeMove(0));
    EXPECT_FALSE(state.makeMove(0));
    EXPECT_EQ(state.getBoard().getCell(0), Marker::X);
}

TEST(GameStateTest, PartialBoardReturnsCorrectLegalMoves) {
    GameState state(Marker::X);

    ASSERT_TRUE(state.makeMove(0));
    ASSERT_TRUE(state.makeMove(4));

    EXPECT_EQ(state.legalMoves(), (std::vector<int>{1, 2, 3, 5, 6, 7, 8}));
}

TEST(GameStateTest, FullBoardHasNoLegalMoves) {
    GameState state(Marker::X);

    // X O X
    // X O O
    // O X X
    ASSERT_TRUE(state.makeMove(0));
    ASSERT_TRUE(state.makeMove(1));
    ASSERT_TRUE(state.makeMove(2));
    ASSERT_TRUE(state.makeMove(4));
    ASSERT_TRUE(state.makeMove(3));
    ASSERT_TRUE(state.makeMove(5));
    ASSERT_TRUE(state.makeMove(7));
    ASSERT_TRUE(state.makeMove(6));
    ASSERT_TRUE(state.makeMove(8));

    EXPECT_TRUE(state.legalMoves().empty());
}

TEST(GameStateTest, DetectsHorizontalWin) {
    GameState state(Marker::X);

    // X X X
    // O O .
    // . . .
    ASSERT_TRUE(state.makeMove(0));
    ASSERT_TRUE(state.makeMove(3));
    ASSERT_TRUE(state.makeMove(1));
    ASSERT_TRUE(state.makeMove(4));
    ASSERT_TRUE(state.makeMove(2));

    ASSERT_TRUE(state.winner().has_value());
    EXPECT_EQ(state.winner().value(), Marker::X);
}

TEST(GameStateTest, DetectsVerticalWin) {
    GameState state(Marker::X);

    // O X X
    // O X .
    // O . .
    ASSERT_TRUE(state.makeMove(1));
    ASSERT_TRUE(state.makeMove(0));
    ASSERT_TRUE(state.makeMove(2));
    ASSERT_TRUE(state.makeMove(3));
    ASSERT_TRUE(state.makeMove(4));
    ASSERT_TRUE(state.makeMove(6));

    ASSERT_TRUE(state.winner().has_value());
    EXPECT_EQ(state.winner().value(), Marker::O);
}

TEST(GameStateTest, DetectsMainDiagonalWin) {
    GameState state(Marker::X);

    // X O O
    // . X .
    // . . X
    ASSERT_TRUE(state.makeMove(0));
    ASSERT_TRUE(state.makeMove(1));
    ASSERT_TRUE(state.makeMove(4));
    ASSERT_TRUE(state.makeMove(2));
    ASSERT_TRUE(state.makeMove(8));

    ASSERT_TRUE(state.winner().has_value());
    EXPECT_EQ(state.winner().value(), Marker::X);
}

TEST(GameStateTest, DetectsOtherDiagonalWin) {
    GameState state(Marker::X);

    // O O X
    // . X .
    // X . .
    ASSERT_TRUE(state.makeMove(2));
    ASSERT_TRUE(state.makeMove(0));
    ASSERT_TRUE(state.makeMove(4));
    ASSERT_TRUE(state.makeMove(1));
    ASSERT_TRUE(state.makeMove(6));

    ASSERT_TRUE(state.winner().has_value());
    EXPECT_EQ(state.winner().value(), Marker::X);
}

TEST(GameStateTest, DoesNotDetectWinnerWithoutThreeInARow) {
    GameState state(Marker::X);

    // X . .
    // . O .
    // . . .
    ASSERT_TRUE(state.makeMove(0));
    ASSERT_TRUE(state.makeMove(4));

    EXPECT_FALSE(state.winner().has_value());
}

TEST(GameStateTest, WinningStateIsTerminal) {
    GameState state(Marker::X);

    // X X X
    // O O .
    // . . .
    ASSERT_TRUE(state.makeMove(0));
    ASSERT_TRUE(state.makeMove(3));
    ASSERT_TRUE(state.makeMove(1));
    ASSERT_TRUE(state.makeMove(4));
    ASSERT_TRUE(state.makeMove(2));

    EXPECT_TRUE(state.isTerminal());
}

TEST(GameStateTest, DrawIsTerminal) {
    GameState state(Marker::X);

    // X O X
    // X O O
    // O X X
    ASSERT_TRUE(state.makeMove(0));
    ASSERT_TRUE(state.makeMove(1));
    ASSERT_TRUE(state.makeMove(2));
    ASSERT_TRUE(state.makeMove(4));
    ASSERT_TRUE(state.makeMove(3));
    ASSERT_TRUE(state.makeMove(5));
    ASSERT_TRUE(state.makeMove(7));
    ASSERT_TRUE(state.makeMove(6));
    ASSERT_TRUE(state.makeMove(8));

    EXPECT_TRUE(state.isTerminal());
    EXPECT_FALSE(state.winner().has_value());
}

TEST(GameStateTest, IncompleteGameIsNotTerminal) {
    GameState state(Marker::X);

    // X . .
    // . O .
    // . . .
    ASSERT_TRUE(state.makeMove(0));
    ASSERT_TRUE(state.makeMove(4));

    EXPECT_FALSE(state.isTerminal());
}

TEST(GameStateTest, MoveAfterWinIsRejected) {
    GameState state(Marker::X);

    // X X X
    // O O .
    // . . .
    ASSERT_TRUE(state.makeMove(0));
    ASSERT_TRUE(state.makeMove(3));
    ASSERT_TRUE(state.makeMove(1));
    ASSERT_TRUE(state.makeMove(4));
    ASSERT_TRUE(state.makeMove(2));
    ASSERT_TRUE(state.isTerminal());

    const Marker turnAfterWin = state.getCurrentTurn();
    EXPECT_FALSE(state.makeMove(8));
    EXPECT_EQ(state.getBoard().getCell(8), Marker::Empty);
    EXPECT_EQ(state.getCurrentTurn(), turnAfterWin);
}

TEST(GameStateTest, WinningStateHasNoLegalMoves) {
    GameState state(Marker::X);

    // X X X
    // O O .
    // . . .
    ASSERT_TRUE(state.makeMove(0));
    ASSERT_TRUE(state.makeMove(3));
    ASSERT_TRUE(state.makeMove(1));
    ASSERT_TRUE(state.makeMove(4));
    ASSERT_TRUE(state.makeMove(2));

    EXPECT_TRUE(state.legalMoves().empty());
}
