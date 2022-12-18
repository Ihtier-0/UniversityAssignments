#ifndef CANONICALADAPTER_H
#define CANONICALADAPTER_H

#include <functional>
#include <memory>

#include "canonicalsolver.h"
#include "globals.h"

class CanonicalAdapter;
using CanonicalAdapterUniquePtr = std::unique_ptr<CanonicalAdapter>;

class CanonicalAdapter {
private:
  CanonicalAdapter() = delete;
  CanonicalAdapter(const CanonicalAdapter &) = delete;
  CanonicalAdapter(CanonicalAdapter &&) = delete;
  CanonicalAdapter operator=(const CanonicalAdapter &) = delete;
  CanonicalAdapter operator=(CanonicalAdapter &&) = delete;

  CanonicalAdapter(const SolveContext &context,
                   CanonicalSolverUniquePtr solver);

public:
  static CanonicalAdapterUniquePtr create(const SolveContext &context,
                                          SolverCreator creator);

  VectorCoefficients calculate(const CalculateCallback &сallback = {});

private:
  static CanonicalContext toCanonical(const SolveContext &context);

  VectorCoefficients fromCanonical(const VectorCoefficients &context);

  SolveContext mContext;
  CanonicalSolverUniquePtr mSolver;
};

#endif // CANONICALADAPTER_H
