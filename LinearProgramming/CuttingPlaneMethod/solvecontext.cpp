#include "solvecontext.h"

bool valid(const SolveContext &context) {
  if (context.type == OptimizationType::Unknown) {
    return false;
  }

  const Size cols = context.constraints.at(0).coefficients.size();

  if (context.objectiveFunctionCoefficients.size() != cols) {
    return false;
  }

  for (const Constraint &c : context.constraints) {
    if (c.sign == Sign::Unknown || c.coefficients.size() != cols) {
      return false;
    }
  }

  return true;
}

bool valid(const CanonicalContext &context) {
  const Size rows = context.constraintsCoefficients.size();
  const Size cols = context.constraintsCoefficients.at(0).size();

  if (context.constraintsConstants.size() != rows) {
    return false;
  }

  if (context.objectiveFunctionCoefficients.size() != cols) {
    return false;
  }

  for (const VectorCoefficients &row : context.constraintsCoefficients) {
    if (row.size() != cols) {
      return false;
    }
  }

  for (const Real &c : context.constraintsConstants) {
    if (c < 0) {
      return false;
    }
  }

  return true;
}
