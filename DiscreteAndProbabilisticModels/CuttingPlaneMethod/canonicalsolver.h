#ifndef CANONICALSOLVER_H
#define CANONICALSOLVER_H

#include <functional>
#include <memory>

#include "globals.h"
#include "solvecontext.h"

class CanonicalSolver;
using CanonicalSolverUniquePtr = std::unique_ptr<CanonicalSolver>;

using CalculateCallback =
    std::function<void(const int &step, const CanonicalContext &context)>;

void printCallback(const int &step, const CanonicalContext &context);

using SolverCreator =
    std::function<CanonicalSolverUniquePtr(const CanonicalContext &)>;

class CanonicalSolver {
public:
  enum EndType {
    End,
    NoEnd,
    Infinity,
    EmptySet,
  };
  static std::string toString(const EndType endType);

  CanonicalSolver(const CanonicalContext &context);
  virtual ~CanonicalSolver();

  virtual std::pair<EndType, VectorCoefficients>
  calculate(const CalculateCallback &calculateCallback = {}) = 0;

  constexpr static const Size invalidIndex = static_cast<Size>(-1); // shit

protected:
  CanonicalContext mContext;
};

#endif // CANONICALSOLVER_H
