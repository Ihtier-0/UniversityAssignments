#include "canonicalsolver.h"

#include "coefficientsutils.h"

CanonicalSolver::~CanonicalSolver() {}

CanonicalSolver::CanonicalSolver(const CanonicalContext &context)
    : mContext(context) {}

void printCallback(const int &step, const CanonicalContext &context) {
  std::cout << "step:" << step << std::endl;
  std::cout << "constraintsCoefficients:\n"
            << context.constraintsCoefficients << std::endl;
  std::cout << "constraintsConstants:\n"
            << context.constraintsConstants << std::endl;
  std::cout << "objectiveFunctionCoefficients:\n"
            << context.objectiveFunctionCoefficients << std::endl;
  std::cout << std::endl;
}
