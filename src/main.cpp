#include <iostream>
#include <board/check.hpp>
#include <board/board.hpp>
#include <board/generators.hpp>

int main() 
{

  auto b = checks::create_random_board();
  auto str_b = checks::serialize_board(b);
  std::cout << str_b << std::endl;

  std::cout << " got here " << std::endl;
  std::cout << checks::check_board(b) << std::endl;
}
