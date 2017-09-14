/*
 * board.cpp
 *
 *  Created on: Sep 13, 2017
 *      Author: bramsden
 */
#include <gtest/gtest.h>
#include <iostream>
#include <algorithm>
#include <array>

#include <board/board.hpp>
#include <board/check.hpp>

namespace {
  const Board::BoardRawData boardData = {4 , 3, 5, 2, 6, 9, 7, 8 , 1,
                               6, 8 ,2, 5, 7 , 1, 4, 9 , 3,
                               1, 9, 7, 8, 3, 4, 5, 6, 2,
                               8, 2, 6, 1, 9, 5, 3, 4, 7,
                               3, 7, 4, 6, 8, 2, 9, 1, 5,
                               9, 5, 1, 7, 4, 3, 6, 2, 8,
                               5, 1, 9, 3, 2, 6, 8, 7, 4,
                               2, 4, 8, 9, 5, 7, 1, 3, 6,
                               7, 6, 3, 4, 1, 8, 2, 5, 9};

  const Board::BoardRawData incorrectBoard = {4 , 3, 5, 2, 6, 9, 7, 8 , 2,
                               6, 8 ,2, 5, 7 , 1, 4, 9 , 3,
                               1, 9, 7, 8, 3, 4, 5, 6, 2,
                               8, 5, 6, 1, 9, 5, 3, 4, 7,
                               3, 7, 4, 6, 8, 2, 9, 1, 5,
                               9, 5, 1, 7, 4, 3, 6, 2, 8,
                               5, 1, 9, 3, 2, 6, 8, 7, 4,
                               2, 4, 8, 9, 5, 7, 1, 3, 6,
                               7, 6, 3, 4, 1, 8, 2, 5, 9};
}

TEST(BoardTest, BasicTests) {
  auto b = Board();
  b.set(0, 1, 1);
  ASSERT_EQ(b.get(0, 1), 1);

  // check subboard
  auto subBoard = b.get_subboard(0, 0);
  ASSERT_EQ(subBoard.get(0, 1), 1);

  subBoard.set(0, 2, 3);
  ASSERT_EQ(b.get(0,2), 3);
}

TEST(BoardTest, BasicSliceTests) {
  auto b = Board(boardData);
  auto get_row_values = [] (Board b, size_t idx) {
    auto slice = b.get_row(idx);
    std::array<int, Board::ROWS> slice_values;
    std::copy(slice.begin(), slice.end(), slice_values.begin());
    return slice_values;
  };


  auto correct_slice_values = std::array<int, Board::ROWS> {4 , 3, 5, 2, 6, 9, 7, 8 , 1};
  ASSERT_EQ(get_row_values(b, 0), correct_slice_values);

  correct_slice_values = std::array<int, Board::ROWS> {9, 5, 1, 7, 4, 3, 6, 2, 8};
  ASSERT_EQ(get_row_values(b, 5), correct_slice_values);

  auto get_col_values = [] (Board b, size_t idx) {
    auto slice = b.get_column(idx);
    std::array<int, Board::ROWS> slice_values;
    std::copy(slice.begin(), slice.end(), slice_values.begin());
    return slice_values;
  };

  correct_slice_values = std::array<int, Board::ROWS> {4,6,1,8,3,9,5,2,7};
  ASSERT_EQ(get_col_values(b, 0), correct_slice_values);

  correct_slice_values = std::array<int, Board::ROWS> {5,2,7,6,4,1,9,8,3};
  ASSERT_EQ(get_col_values(b, 2), correct_slice_values);

  auto get_sub_board = [] (Board b, size_t idx_x, size_t idx_y) {
    auto slice = b.get_subboard(idx_x, idx_y);
    std::array<int, Board::ROWS> slice_values;
    std::copy(slice.begin(), slice.end(), slice_values.begin());
    return slice_values;
  };

  correct_slice_values = std::array<int, Board::ROWS> {4,3,5,6,8,2,1,9,7};
  ASSERT_EQ(get_sub_board(b, 0, 0), correct_slice_values);

  correct_slice_values = std::array<int, Board::ROWS> {7, 8, 1, 4, 9, 3, 5, 6, 2};
  ASSERT_EQ(get_sub_board(b, 0, 2), correct_slice_values);
};

TEST(BoardTest, BasicCheckTests) {
  auto b = Board(boardData);

  ASSERT_TRUE(checks::check_board(b));
  ASSERT_EQ(checks::num_errors_board(b), 0);

  for(auto& r : b.rows()) {
    ASSERT_TRUE(checks::check<BoardSlice>(r));
    ASSERT_EQ(checks::num_errors<BoardSlice>(r), 0);
  }

  for(auto& c : b.cols()) {
    ASSERT_TRUE(checks::check<BoardSlice>(c));
    ASSERT_EQ(checks::num_errors<BoardSlice>(c), 0);
  }

  for(auto& sb : b.subboards()) {
    ASSERT_TRUE(checks::check<SubBoard>(sb));
    ASSERT_EQ(checks::num_errors<SubBoard>(sb), 0);
  }

  ASSERT_TRUE(checks::board_set(b));
}

TEST(BoardTest, BasicCheckTests_incorrect) {
  auto b = Board(incorrectBoard);

  ASSERT_FALSE(checks::check_board(b));
  ASSERT_EQ(checks::num_errors_board(b), 6);
}


