#include <iostream>
#include <board/check.hpp>
#include <board/board.hpp>
#include <board/generators.hpp>
#include <board/genetic.hpp>
#include <board/genetic_sodoku.hpp>

#include <chrono>

int main() 
{
  #define DEBUG 1

  // aim is to create a random board
  auto b = checks::create_random_board ();
  auto str_b = checks::serialize_board(b);
  std::cout << " sample board (created with recursion) \n";
  std::cout << " valid: " << checks::check_board(b) << "\n";
  std::cout << str_b << "\n";

  auto genetic = sodoku_genetic::get_genetic_sodoku();
  auto out = genetic.genetic_search(1000);
  str_b = checks::serialize_board(out.individual);

  std::cout << " sample board (created with genetic search) \n";
  std::cout << " valid: " << checks::check_board(out.individual) << "\n";
  std::cout << str_b << "\n";

  auto test_with_random_matrix = [] () {
  auto start = std::chrono::steady_clock::now();
  constexpr long num_runs = 10000;
  for(auto i : Range<num_runs> () ) {
    checks::create_random_board ();
  }
  auto end = std::chrono::steady_clock::now();

  auto micro_seconds = std::chrono::duration_cast <std::chrono::microseconds> (end - start).count();
  std::cout << " took " << micro_seconds << " microseconds to generate " << num_runs << " sodokus \n" << std::endl;
  std::cout << " average time " << micro_seconds/num_runs << std::endl;
  };

  test_with_random_matrix();

}
