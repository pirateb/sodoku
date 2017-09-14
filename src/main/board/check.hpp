#pragma once

#include "board.hpp"
#include <bitset>
#include <tuple>
#include <string>
#include <iostream>

namespace checks {
  constexpr size_t VALID_SIZE = 9;

  namespace {
    std::bitset<VALID_SIZE> get_all_values() {
      return std::bitset<VALID_SIZE> () .set();
    }

    std::bitset<VALID_SIZE> no_values() {
      return std::bitset<VALID_SIZE> ();
    }

  };

  bool check_board(Board& board);

  template<typename T>
  bool check(T t) {
    auto all_values = get_all_values();
    for(const auto& v : t) {
      if(!all_values.test(v - 1)) return false;
      all_values.reset(v - 1);
    }
    return true;
  }

  template<typename T>
  int num_errors(T t) {
    auto all_values = get_all_values();
    int num_error = 0;
    for(const auto& v : t) {
      if(!all_values.test(v - 1)) {
        num_error += 1;
      } else {
        all_values.reset(v - 1);
      }
    }
    return num_error;
  }

  int num_errors_board(Board& board);
  bool board_set(Board& board);
  std::bitset<VALID_SIZE> get_valid_guesses(Board& board, size_t idx_x, size_t idx_y);

  std::tuple<size_t, size_t> convert_index(size_t idx);

  Board create_random_board();
  std::string serialize_board(Board& board);

  template<typename Generator>
  std::tuple<Board, bool> solve_recursive(Board board) {
    if(checks::board_set(board)) {
      if(checks::check_board(board))
        return std::make_tuple(board, true);
      else
        return std::make_tuple(board, false);
    }

    // board is not set. find first not valid entry
    auto it = std::find(board.raw_data().begin(), board.raw_data().end(), Board::NOT_SET);
    auto idx = std::distance(board.raw_data().begin(), it);
    auto idx_tuple = convert_index(idx);
    auto idx_x = std::get<0>(idx_tuple);
    auto idx_y = std::get<1>(idx_tuple);

    // get possible guesses
    auto possible_guesses = get_valid_guesses(board, idx_x, idx_y);
    for(auto i : Generator()) {
      if(possible_guesses.test(i)) {
        // this is a possible guess
        auto new_board = board;
        new_board.set(idx_x, idx_y, i + 1);
        auto possible_out = solve_recursive<Generator>(new_board);
        if (std::get<1>( possible_out)) {
          return possible_out;
        }
      }
    }

    return std::make_tuple(board, false);
  }
}

