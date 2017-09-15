#pragma once

#include <chrono>

// small class to generate random numbers
// global gen - change for thread safety

class RandomUniform {
public:
  RandomUniform()
    : distribution{0.0, 1.0}
  {
  }

  double rand() { return distribution(gen); }

private:
  std::uniform_real_distribution<double> distribution;
  std::default_random_engine gen{static_cast<long unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())};
};

class RandomIntUniform {
public:
  RandomIntUniform(int min, int max) :
    distribution{min, max}
  {
  }

  int rand() { return distribution(gen); }

private:
  std::uniform_int_distribution<int> distribution;
  std::default_random_engine gen{static_cast<long unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())};
};
