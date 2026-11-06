#include <gtest/gtest.h>

#include "Board.hpp"
#include "Marker.hpp"

TEST(BoardTest, NewBoardIsEmpty) {
    Board board;

    for (int index = 0; index < 9; ++index) {
        EXPECT_EQ(board.getCell(index), Marker::Empty);
    }
}

TEST(BoardTest, NewBoardIsNotFull) {
    Board board;

    EXPECT_FALSE(board.isFull());
}

TEST(BoardTest, PlaceMarkerInEmptyCell) {
    Board board;

    EXPECT_TRUE(board.place(4, Marker::X));
    EXPECT_EQ(board.getCell(4), Marker::X);
}

TEST(BoardTest, CannotOverwriteOccupiedCell) {
    Board board;

    EXPECT_TRUE(board.place(4, Marker::X));
    EXPECT_FALSE(board.place(4, Marker::O));
    EXPECT_EQ(board.getCell(4), Marker::X);
}

TEST(BoardTest, RejectsIndexBelowBoard) {
    Board board;

    EXPECT_FALSE(board.place(-1, Marker::X));
}

TEST(BoardTest, RejectsIndexAboveBoard) {
    Board board;

    EXPECT_FALSE(board.place(9, Marker::X));
}

TEST(BoardTest, GetCellRejectsIndexBelowBoard) {
    Board board;

    EXPECT_EQ(board.getCell(-1), Marker::Empty);
}

TEST(BoardTest, GetCellRejectsIndexAboveBoard) {
    Board board;

    EXPECT_EQ(board.getCell(9), Marker::Empty);
}

TEST(BoardTest, AcceptsBoundaryIndices) {
    Board board;

    EXPECT_TRUE(board.place(0, Marker::X));
    EXPECT_TRUE(board.place(8, Marker::O));
    EXPECT_EQ(board.getCell(0), Marker::X);
    EXPECT_EQ(board.getCell(8), Marker::O);
}

TEST(BoardTest, PartialBoardIsNotFull) {
    Board board;

    EXPECT_TRUE(board.place(0, Marker::X));
    EXPECT_TRUE(board.place(1, Marker::O));
    EXPECT_TRUE(board.place(4, Marker::X));

    EXPECT_FALSE(board.isFull());
}

TEST(BoardTest, FullBoardIsFull) {
    Board board;

    for (int index = 0; index < 9; ++index) {
        EXPECT_TRUE(board.place(index, Marker::X));
    }

    EXPECT_TRUE(board.isFull());
}
