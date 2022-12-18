#include <iostream>

#include "canonicaladapter.h"
#include "coefficientsutils.h"
#include "globals.h"
#include "simplexalgorithm.h"

int main() {
  try {
    {
      // 2 * x1 + 3 * x2 + 0 * x3 - x4 -> max

      // 2 * x1     - x2 +  0 - 2 * x4 <= 16
      // 3 * x1 + 2 * x2 + x3 - 3 * x4  = 18
      //   - x1 + 3 * x2 +  0 + 4 * x4 <= 24

      //

      // 2 * x1 + 3 * x2 + 0 * x3 - x4 -> max

      // 2 * x1     - x2 +  0 - 2 * x4 + x5 +  0 = 16
      // 3 * x1 + 2 * x2 + x3 - 3 * x4 +  0 +  0 = 18
      //   - x1 + 3 * x2 +  0 + 4 * x4 +  0 + x6 = 24

      CanonicalContext context;
      context.objectiveFunctionCoefficients = {2.0f,  3.0f, 0.0f,
                                               -1.0f, 0.0f, 0.0f};

      // clang-format off
      context.constraintsCoefficients = {
          { 2.0f, -1.0f,  0.0f, -2.0f, 1.0f, 0.0f},
          { 3.0f,  2.0f,  1.0f, -3.0f, 0.0f, 0.0f},
          {-1.0f,  3.0f,  0.0f,  4.0f, 0.0f, 1.0f}
      };
      // clang-format on

      context.constraintsConstants = {16.0f, 18.0f, 24.0f};

      CanonicalSolverUniquePtr simplex = SimplexAlgorithm::create(context);

      if (!simplex) {
        return -1;
      }

      VectorCoefficients answer = simplex->calculate(printCallback);

      std::cout << "answer: " << answer << std::endl;
    }

    {
      // 2 * x1 + 3 * x2 + 0 * x3 - x4 -> max

      // 2 * x1     - x2 +  0 - 2 * x4 <= 16
      // 3 * x1 + 2 * x2 + x3 - 3 * x4  = 18
      //   - x1 + 3 * x2 +  0 + 4 * x4 <= 24

      SolveContext context;
      context.objectiveFunctionCoefficients = {2.0f, 3.0f, 0.0f, -1.0f};
      context.type = OptimizationType::Max;

      // clang-format off
      context.constraints = {
          {{ 2.0f, -1.0f,  0.0f, -2.0f}, Sign::le, 16},
          {{ 3.0f,  2.0f,  1.0f, -3.0f}, Sign::eq, 18},
          {{-1.0f,  3.0f,  0.0f,  4.0f}, Sign::le, 24}
      };
      // clang-format on

      CanonicalAdapterUniquePtr simplex =
          CanonicalAdapter::create(context, SimplexAlgorithm::create);

      if (!simplex) {
        return -1;
      }

      VectorCoefficients answer = simplex->calculate(printCallback);

      std::cout << "answer: " << answer << std::endl;
    }

    return 0;
  } catch (std::exception &exception) {
    std::cout << exception.what() << std::endl;
  } catch (...) {
    std::cout << "unknown exception" << std::endl;
  }

  return -1;
}
