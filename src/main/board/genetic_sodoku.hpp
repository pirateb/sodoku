#pragma

#include "board.hpp"
#include "random.hpp"
#include <functional>
#include <algorithm>
#include "check.hpp"

namespace sodoku_genetic {
  // give concrete implementations for everything that is needed in genetic search

  using SodokuIndividual = Board;
  using SodokuEncoding = Board::BoardRawData;

  using SodokuEncoder = std::function<SodokuEncoding(SodokuIndividual)>;
  using SodokuDecoder = std::function<SodokuIndividual(SodokuEncoding)>;
  using SodokuRandomEncoder = std::function<SodokuEncoding()>;
  using SodokuScorer = std::function<double(SodokuIndividual)>;
  using SodokuCrossOver = std::function<SodokuEncoding(SodokuEncoding, SodokuEncoding)>;
  using SodokuMutate = std::function<SodokuEncoding(SodokuEncoding)>;

  constexpr size_t SODOKU_POP_SIZE = 100;

  constexpr double CROSS_OVER_PERCENTAGE = 0.5;
  constexpr double MUTATE_PER = 0.1;

  SodokuEncoder get_sodoku_encoder() {
    return [](SodokuIndividual board) {
      return board.raw_data();
    };
  }

  SodokuDecoder get_sodoku_decoder() {
    return [](SodokuEncoding rawBoardData) {
      return Board{rawBoardData};
    };
  }

  // POTENTIALLY SLOW - SEED IS REINITIALISED
  SodokuRandomEncoder get_sodoku_random_encoder() {
    return []() {
      auto rnd = RandomIntUniform(1, 9);
      SodokuEncoding rawData;
      for(auto&v : rawData) {
        v = rnd.rand();
      }
      return rawData;
    };
  }

  SodokuScorer get_sodoku_scorer() {
    return [] (SodokuIndividual individual) {
      return static_cast<double> (checks::num_errors_board(individual));
    };
  }

  // POTENTIALLY SLOW - SEED IS REINITIALISED
  SodokuCrossOver get_sodoku_crossover() {
    return [] (SodokuEncoding encoding1, SodokuEncoding encoding2) {
      auto rnd = RandomUniform();
      SodokuEncoding rawData;
      for(size_t i = 0; i < rawData.size(); i++) {
        if(rnd.rand() < CROSS_OVER_PERCENTAGE)
          rawData[i] = encoding1[i];
        else
          rawData[i] = encoding2[i];
      }
      return rawData;
    };
  }

  // POTENTIALLY SLOW - SEED IS REINITIALISED
  SodokuMutate get_sodoku_mutate() {
    return [] (SodokuEncoding encoding) {
      auto rnd = RandomUniform();
      auto rnd_int = RandomIntUniform(1, 9);

    for(size_t i = 0; i < encoding.size(); i++) {
      if(rnd.rand() < MUTATE_PER) {
        encoding[i] = rnd_int.rand();
      }
      /*
      auto b = Board {encoding};
      auto idx = checks::convert_index(i);
      auto random_valid_guess = checks::get_random_valid_guess(b, std::get<0>(idx), std::get<1>(idx));
      if(random_valid_guess != Board::NOT_SET) encoding[i] = random_valid_guess;
      */
    }
      return encoding;
    };
  }

  using SodokuGenetic = Genetic<SodokuIndividual, SodokuEncoding, SODOKU_POP_SIZE>;
  SodokuGenetic get_genetic_sodoku() {
    return SodokuGenetic {
      get_sodoku_encoder(),
      get_sodoku_decoder(),
      get_sodoku_random_encoder(),
      get_sodoku_scorer(),
      get_sodoku_crossover(),
      get_sodoku_mutate()
    };
  };
}
