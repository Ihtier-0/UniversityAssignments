#ifndef CANONICALADAPTER_H
#define CANONICALADAPTER_H

#include <functional>
#include <memory>

#include "globals.h"
#include "simplexalgorithm.h"

class CanonicalAdapter;
using CanonicalAdapterUniquePtr = std::unique_ptr<CanonicalAdapter>;

class CanonicalAdapter {
public:
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

private:
  CanonicalAdapter() = delete;
  CanonicalAdapter(const CanonicalAdapter &) = delete;
  CanonicalAdapter(CanonicalAdapter &&) = delete;
  CanonicalAdapter operator=(const CanonicalAdapter &) = delete;
  CanonicalAdapter operator=(CanonicalAdapter &&) = delete;

  CanonicalAdapter(const SolveContext &context,
                   SimplexAlgorithmUniquePtr simplex);

public:
  static CanonicalAdapterUniquePtr create(const SolveContext &solveContext);

  VectorCoefficients
  calculate(const SimplexAlgorithm::CalculateCallback &сallback = {});

private:
  static SimplexAlgorithm::SolveContext
  toCanonical(const CanonicalAdapter::SolveContext &context);

  VectorCoefficients fromCanonical(const VectorCoefficients &context);

  SolveContext mContext;
  SimplexAlgorithmUniquePtr mSimplex;
};

#endif // CANONICALADAPTER_H
