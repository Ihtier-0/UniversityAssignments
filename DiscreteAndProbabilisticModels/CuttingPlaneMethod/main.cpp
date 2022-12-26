#include <iostream>

#include "canonicaladapter.h"
#include "coefficientsutils.h"
#include "cuttingplanemethod.h"
#include "globals.h"
#include "simplexalgorithm.h"

int main() {
  try {
    // // SimplexAlgorithm
    // {
    //   // 2 * x1 + 3 * x2 + 0 * x3 - x4 -> max

    //   // 2 * x1     - x2 +  0 - 2 * x4 <= 16
    //   // 3 * x1 + 2 * x2 + x3 - 3 * x4  = 18
    //   //   - x1 + 3 * x2 +  0 + 4 * x4 <= 24

    //   SolveContext context;
    //   context.objectiveFunctionCoefficients = {
    //       2.0f,
    //       3.0f,
    //       0.0f,
    //       -1.0f,
    //   };
    //   context.type = OptimizationType::Max;

    //   context.constraints = {
    //       {{2.0f, -1.0f, 0.0f, -2.0f}, Sign::le, 16},
    //       {{3.0f, 2.0f, 1.0f, -3.0f}, Sign::eq, 18},
    //       {{-1.0f, 3.0f, 0.0f, 4.0f}, Sign::le, 24},
    //   };

    //   CanonicalAdapterUniquePtr simplex =
    //       CanonicalAdapter::create(context, SimplexAlgorithm::create);

    //   if (!simplex) {
    //     return EXIT_FAILURE;
    //   }

    //   std::pair<CanonicalSolver::EndType, VectorCoefficients> answer =
    //       simplex->calculate(printCallback);

    //   if (answer.first == CanonicalSolver::End) {
    //     std::cout << "answer: " << answer.second << std::endl;
    //   } else {
    //     std::cout << "answer: " << CanonicalSolver::toString(answer.first)
    //               << std::endl;
    //   }
    // }

    // std::cout << std::endl;

    // CuttingPlaneMethod
    {
      // 2 * x1 + 4 * x2 - 8 * x3 -> max

      // x1 + 3 * x2 - 9 * x3 >= 27
      // x1 - 4 * x2 + 8 * x3 >= 12
      // x1 + 1 * x2 + 1 * x3 >= 3

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

    return EXIT_SUCCESS;
  } catch (std::exception &exception) {
    std::cout << exception.what() << std::endl;
  } catch (...) {
    std::cout << "unknown exception" << std::endl;
  }

  return EXIT_FAILURE;
}
