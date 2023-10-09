#ifndef CUTTINGPLANEMETHOD_H
#define CUTTINGPLANEMETHOD_H

#include "canonicalsolver.h"

class CuttingPlaneMethod : public CanonicalSolver {
private:
  CuttingPlaneMethod() = delete;
  CuttingPlaneMethod(const CuttingPlaneMethod &) = delete;
  CuttingPlaneMethod(CuttingPlaneMethod &&) = delete;
  CuttingPlaneMethod operator=(const CuttingPlaneMethod &) = delete;
  CuttingPlaneMethod operator=(CuttingPlaneMethod &&) = delete;

  CuttingPlaneMethod(const CanonicalContext &context, SolverCreator creator);
  virtual ~CuttingPlaneMethod() override;

public:
  static CanonicalSolverUniquePtr create(const CanonicalContext &context,
                                         SolverCreator creator);

  virtual std::pair<EndType, VectorCoefficients>
  calculate(const CalculateCallback &callback = {}) override;

private:
  SolverCreator mCreator;
};

#endif // CUTTINGPLANEMETHOD_H
