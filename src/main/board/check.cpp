#include "check.hpp"
#include <algorithm>
#include <functional>
#include <random>
#include <sstream>
#include "generators.hpp"

bool checks::check_board(Board& board) {
  for(auto& subboard: board.subboards()) {
    if( !checks::check<SubBoard>(subboard) ) return false;
  }

  for(auto& row: board.rows()) {
    if( !checks::check<BoardSlice>(row) ) return false;
  }

  for(auto& col: board.cols()) {
    if( !checks::check<BoardSlice>(col) ) return false;
  }

  return true;
}

int checks::num_errors_board(Board& board) {
  int num_error = 0;
  for(auto& subboard: board.subboards()) {
    num_error += checks::num_errors<SubBoard>(subboard);
  }

  for(auto& row: board.rows()) {
    num_error += checks::num_errors<BoardSlice>(row);
  }

  for(auto& col: board.cols()) {
    num_error += checks::num_errors<BoardSlice>(col);
  }

  return num_error;
}

bool checks::board_set(Board& board) {
  return std::all_of(board.raw_data().begin(), board.raw_data().end(), [](int x) { return x != Board::NOT_SET;});
}

std::bitset<checks::VALID_SIZE> checks::get_valid_guesses(Board& board, size_t idx_x, size_t idx_y) {
  auto val = board.get(idx_x, idx_y);
  auto valid = get_all_values();

  std::for_each(board.get_column(idx_y).begin(),
      board.get_column(idx_y).end(),
      [&valid](int val) { if(val != Board::NOT_SET) valid.reset(val - 1);}
  );

  std::for_each(board.get_row(idx_x).begin(),
      board.get_row(idx_y).end(),
      [&valid](int val) { if(val != Board::NOT_SET) valid.reset(val - 1);}
  );

  std::for_each(board.get_subboard(idx_x / SubBoard::SUB_BOARD_ROWS, idx_y / SubBoard::SUB_BOARD_ROWS).begin(),
      board.get_subboard(idx_x, idx_y).end(),
      [&valid, idx_x, idx_y](int val) { if(val != Board::NOT_SET) valid.reset(val - 1);}
  );

  return valid;
}

int checks::get_random_valid_guess(Board& board, size_t idx_x, size_t idx_y)
{
  auto valid_guesses = get_valid_guesses(board, idx_x, idx_y);
  if(!valid_guesses.any()) return Board::NOT_SET;

  auto rng = RandomIntUniform(0, VALID_SIZE - 1).rand();
  for(size_t i = 0; i < valid_guesses.size(); i++) {
    auto pos = (rng + i) % valid_guesses.size();
    if(valid_guesses.test(pos)) return pos;
  }

  std::cout << "why?" << std::endl;
  return Board::NOT_SET; // shouldnt reach here
}

Board checks::create_random_board() {
  auto b = Board();
  auto out = solve_recursive<RandomRange<VALID_SIZE>> (b);
  return std::get<0>(out);
}

std::string checks::serialize_board(Board& board) {
  std::stringstream ss;
  for(auto& row : board.rows()) {
    for(auto& v: row) {
      ss << v;
    }
    ss << "\n";
  }
  return ss.str();
}

std::tuple<size_t, size_t> checks::convert_index(size_t idx) {
   auto idx_x = idx / Board::ROWS;
   auto idx_y = idx % Board::ROWS;
   return std::make_tuple(idx_x, idx_y);
 }
