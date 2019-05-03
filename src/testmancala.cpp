#include <gtest/gtest.h>
#include "MancalaBoard.h"

TEST(MancalaBoardTest, DefaultBoardTest) {
    // Needs to test that default board is correct
    CMancalaBoard MancalaBoard;
    EXPECT_EQ(std::string(MancalaBoard), "P1          PITs\n"
                                         "      5   4   3   2   1\n"
                                         "/---------------------------\\\n"
                                         "|   | 4 | 4 | 4 | 4 | 4 |   |\n"
                                         "| 0 |-------------------| 0 |\n"
                                         "|   | 4 | 4 | 4 | 4 | 4 |   |\n"
                                         "\\---------------------------/\n"
                                         "      1   2   3   4   5\n"
                                         "             PITS          P2\n")

}

TEST(MancalaBoardTest, SetBoardTest) {
    // Needs to test that setting up an initial board is correct
}

TEST(MancalaBoardTest, ResetBoardTest) {
    // Needs to test that resetting a board is correct
}

TEST(MancalaBoardTest, BasicMoveTest) {
    // Needs to test that basic non-scoring moves are correct
}

TEST(MancalaBoardTest, ScoringMoveTest) {
    // Needs to test that basic scoring, non double moves, non steals are correct
}

TEST(MancalaBoardTest, DoubleMoveTest) {
    // Needs to test that double move is correct
}

TEST(MancalaBoardTest, StealMoveTest) {
    // Needs to test that stealing move works correctly
}

TEST(MancalaBoardTest, CantMoveTest) {
    // Needs to test when player can't move correctly
}

TEST(MancalaBoardTest, BadParametersTest) {
    // Needs to check that bad parameters are handled correctly
    CMancalaBoard MancalaBoard;
    //int InputLine = "Hello\n";
    EXPECT_FALSE(0);
}

