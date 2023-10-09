#include <iostream>

#include "canonicaladapter.h"
#include "coefficientsutils.h"
#include "cuttingplanemethod.h"
#include "globals.h"
#include "simplexalgorithm.h"

int main() {
  try {
    // CuttingPlaneMethod
    // example 1
    {
      // 1 * x1 + 2 * x2 -> max

      // 3 * x1 + 4 * x2 <= 7
      // 5 * x1 + 6 * x2 <= 8

      SolveContext context;
      context.objectiveFunctionCoefficients = {
          1.0f,
          2.0f,
      };
      context.type = OptimizationType::Max;

      context.constraints = {
          {{3.0f, 4.0f}, Sign::le, 7},
          {{5.0f, 6.0f}, Sign::le, 8},
      };

      CanonicalAdapterUniquePtr simplex = CanonicalAdapter::create(
          context, std::bind(CuttingPlaneMethod::create, std::placeholders::_1,
                             SimplexAlgorithm::create));

      if (!simplex) {
        return EXIT_FAILURE;
      }

      std::pair<CanonicalSolver::EndType, VectorCoefficients> answer =
          simplex->calculate(printCallback);

      if (answer.first == CanonicalSolver::End) {
        std::cout << "answer: " << answer.second << std::endl;
      } else {
        std::cout << "answer: " << CanonicalSolver::toString(answer.first)
                  << std::endl;
      }
    }

    std::cout << std::endl << std::endl;

    // example 2
    {
      // 1 * x1 + 1 * x2 -> max

      // 1 * x1 + 2 * x2 <= 15
      // 2 * x1 + 1 * x2 <= 17

      SolveContext context;
      context.objectiveFunctionCoefficients = {
          1.0f,
          1.0f,
      };
      context.type = OptimizationType::Max;

      context.constraints = {
          {{1.0f, 2.0f}, Sign::le, 15},
          {{2.0f, 1.0f}, Sign::le, 17},
      };

      CanonicalAdapterUniquePtr simplex = CanonicalAdapter::create(
          context, std::bind(CuttingPlaneMethod::create, std::placeholders::_1,
                             SimplexAlgorithm::create));

      if (!simplex) {
        return EXIT_FAILURE;
      }

      std::pair<CanonicalSolver::EndType, VectorCoefficients> answer =
          simplex->calculate(printCallback);

      if (answer.first == CanonicalSolver::End) {
        std::cout << "answer: " << answer.second << std::endl;
      } else {
        std::cout << "answer: " << CanonicalSolver::toString(answer.first)
                  << std::endl;
      }
    }

    std::cout << std::endl << std::endl;

    // example 3
    {
      // 1 * x1 + 1 * x2 -> max

      // x >= 0.2
      // x <= 0.8
      // y >= 0.2
      // y <= 0.8

      SolveContext context;
      context.objectiveFunctionCoefficients = {
          1.0f,
          1.0f,
      };
      context.type = OptimizationType::Max;

      context.constraints = {
          {{1.0f, 0.0f}, Sign::ge, 0.2f},
          {{1.0f, 0.0f}, Sign::le, 0.8f},
          {{0.0f, 1.0f}, Sign::ge, 0.2f},
          {{0.0f, 1.0f}, Sign::le, 0.8f},
      };

      CanonicalAdapterUniquePtr simplex = CanonicalAdapter::create(
          context, std::bind(CuttingPlaneMethod::create, std::placeholders::_1,
                             SimplexAlgorithm::create));

      if (!simplex) {
        return EXIT_FAILURE;
      }

      std::pair<CanonicalSolver::EndType, VectorCoefficients> answer =
          simplex->calculate(printCallback);

      if (answer.first == CanonicalSolver::End) {
        std::cout << "answer: " << answer.second << std::endl;
      } else {
        std::cout << "answer: " << CanonicalSolver::toString(answer.first)
                  << std::endl;
      }
    }

    return EXIT_SUCCESS;
  } catch (std::exception &exception) {
    std::cout << exception.what() << std::endl;
  } catch (...) {
    std::cout << "unknown exception" << std::endl;
  }

  return EXIT_FAILURE;
}
