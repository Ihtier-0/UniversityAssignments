#ifndef SOLVECONTEXT_H
#define SOLVECONTEXT_H

#include "globals.h"

enum class OptimizationType {
  Unknown,
  Max,
  Min,
};

enum class Sign {
  Unknown,
  eq, // ==
  ge, // >=
  gt, // >
  le, // <=
  lt, // <
  // ne, // !=
};

struct Constraint {
  VectorCoefficients coefficients;
  Sign sign = Sign::Unknown;
  Real constants;
};

struct SolveContext {
  OptimizationType type = OptimizationType::Unknown;
  Vector<Constraint> constraints;
  VectorCoefficients objectiveFunctionCoefficients;
};

bool valid(const SolveContext &context);

struct CanonicalContext {
  MatrixCoefficients constraintsCoefficients;
  VectorCoefficients constraintsConstants;
  VectorCoefficients objectiveFunctionCoefficients;
};

bool valid(const CanonicalContext &context);

#endif // SOLVECONTEXT_H
