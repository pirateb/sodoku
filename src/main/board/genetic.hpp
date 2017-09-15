#pragma once

#include <array>
#include <tuple>
#include <random>
#include <chrono>
#include <functional>

#include "random.hpp"
#include <iostream>

template < typename Individual, typename Encoding, size_t POP_SIZE>
class Genetic
{
public:

  // declare types we will need
  // individual packaged with its fitness
  struct IndividualScores {
    IndividualScores() : individual{}, score{} {}
    IndividualScores(Individual i, double s) : individual{i}, score{s} { };
    Individual            individual;
    double                score;
  };

  // various functions that will need to be provided
  using Encoder = std::function<Encoding(Individual)>;
  using Decoder = std::function<Individual(Encoding)>;
  using RandomEncoder = std::function<Encoding()>;
  using Scorer = std::function<double(Individual)>;
  using CrossOver = std::function<Encoding(Encoding, Encoding)>;
  using Mutate = std::function<Encoding(Encoding)>;
  using Population = std::array<IndividualScores, POP_SIZE>;

  static constexpr double elite_selection_proportion = 0.1;
  static constexpr size_t elite_selection_number = POP_SIZE * elite_selection_proportion;

public:
  Genetic(Encoder encoder,
      Decoder decoder,
      RandomEncoder random_encoder,
      Scorer fitness,
      CrossOver cross_over,
      Mutate mutate)
    : _encoder{encoder}, _decoder{decoder}, _random_encoder{random_encoder}, _fitness{fitness},
      _cross_over{cross_over}, _mutate{mutate}, _current_population{&_population}
  {
    // initial set up of random generator

  }

  void generate_initial_population() {
    for(size_t i = 0; i < POP_SIZE; i++) {
      auto individual = _decoder(_random_encoder());
      auto score = _fitness(individual);
      _population[i] = IndividualScores(individual, score);
    }

    sort_population(_population);
    _current_population = &_population;
  }

  IndividualScores genetic_search(size_t max_runs) {
    // fitness is assumed to be best at 0 and monotonic increasing
    generate_initial_population();

    for(size_t r = 0; r < max_runs; r++) {
      Population& population = current_population();
      Population& target_pop = target_population();

      if(population[0].score == 0) {
        // no need to search, already found best
        return population[0];
      }

      // elite part of the population goes through unhindered
      std::copy_n(population.begin(), elite_selection_number, target_pop.begin());

      for(size_t i = elite_selection_number; i < target_pop.size(); i++) {
        auto& individual1 = roullete_search();
        auto& individual2 = roullete_search();

        auto encoding1 = _encoder(individual1.individual);
        auto encoding2 = _encoder(individual2.individual);

        // cross over and mutate
        Encoding x = _cross_over(encoding1, encoding2);
        Encoding encoding_target = _mutate(x);
        auto individual_target = _decoder(encoding_target);
        auto score = _fitness(individual_target);

        // store in target_population
        target_pop[i] = IndividualScores{individual_target, score};
      }

      // sort target population
      sort_population(target_pop);

      // change buffer
      _current_population = &target_pop;
    }

    return current_population()[0];
  }

private:
  // double buffer on populations
  // i have to modify the population, and i dont want to allocate memmory
  Population                                _population;
  Population                                _population_buffer;
  Population*                               _current_population;

  Encoder                                   _encoder;
  Decoder                                   _decoder;
  RandomEncoder                             _random_encoder;
  Scorer                                    _fitness;
  CrossOver                                 _cross_over;
  Mutate                                    _mutate;
  RandomUniform                             _rnd_uniform;

private:
  // current buffer
  Population& current_population() {
    return *_current_population;
  }

  // other buffer
  Population& target_population() {
    return _current_population == &_population ? _population_buffer : _population;
  }

  void sort_population(Population& population) {
    std::sort(population.begin(),
        population.end(),
        [](IndividualScores& l, IndividualScores& r) {
          return l.score < r.score;
    });
  }

  // Note that everything is a little backwards due to constraint / choice that a fitness of 0 is optimal
  IndividualScores& roullete_search() {
    auto& population = current_population();
    double max_score = population.back().score;

    auto get_total_score = [&population, max_score]() {
      double total_score = 0;

      for(int i = 0; i < population.size(); i++) {
        total_score += (max_score - population[i].score);
      }
      return total_score;
    };

    auto total_score = get_total_score();
    auto inv_total_score = 1.0 / total_score;

    auto crnt_score = 0.0;
    auto rnd = _rnd_uniform.rand();

    for(auto& is: population) {
      crnt_score += ( (max_score - is.score) * inv_total_score);
      if(crnt_score > rnd) {
        return is;
      }
    }

    return *population.end();
  }
};
