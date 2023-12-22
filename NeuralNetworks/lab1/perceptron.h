#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <functional>
#include <vector>

class Perceptron {
public:
  using ActivationFunction = std::function<float(const float sum)>;

  Perceptron(const std::size_t weightsCount, const bool randomWeights = false);

  void randomWeights();

  float recognize(const std::vector<float> &input);

  void update(const float learningRate, const float delta,
              const std::vector<float> &input);

private:
  ActivationFunction mActivationFunction =
      /* Binary step */ [](const float sum) {
        return static_cast<float>(sum >= 0);
      };
  std::vector<float> mWeights;
};

#endif // PERCEPTRON_H
