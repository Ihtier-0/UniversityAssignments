#include "perceptron.h"

#include <cassert>
#include <random>

Perceptron::Perceptron(const size_t weightsCount, const bool randomWeights) {
  mWeights.resize(weightsCount, 0);

  if (randomWeights) {
    this->randomWeights();
  }
}

void Perceptron::randomWeights() {
  const auto seed = 0;
  std::mt19937 rand_engine(seed);
  std::uniform_real_distribution<> distribution(-0.3, 0.3); // 1 / (b - a)

  for (auto &weight : mWeights) {
    weight = distribution(rand_engine);
  }
}

float Perceptron::recognize(const std::vector<float> &input) {
  const auto size = mWeights.size();

  assert((input.size() == size));

  float sum = 0;

  for (std::size_t i = 0; i < size; ++i) {
    sum += mWeights[i] * input[i];
  }

  return mActivationFunction(sum);
}

void Perceptron::update(const float learningRate, const float delta,
                        const std::vector<float> &input) {
  const auto size = mWeights.size();

  for (std::size_t i = 0; i < size; ++i) {
    mWeights[i] += learningRate * delta * input[i];
  }
}
