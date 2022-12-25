#ifndef SIMPLEXALGORITHM_H
#define SIMPLEXALGORITHM_H

#include <functional>
#include <memory>

#include "canonicalsolver.h"
#include "globals.h"
#include "solvecontext.h"

class SimplexAlgorithm : public CanonicalSolver {
private:
  SimplexAlgorithm() = delete;
  SimplexAlgorithm(const SimplexAlgorithm &) = delete;
  SimplexAlgorithm(SimplexAlgorithm &&) = delete;
  SimplexAlgorithm operator=(const SimplexAlgorithm &) = delete;
  SimplexAlgorithm operator=(SimplexAlgorithm &&) = delete;

  SimplexAlgorithm(const CanonicalContext &canonicalContext);
  virtual ~SimplexAlgorithm() override;

public:
  static CanonicalSolverUniquePtr
  create(const CanonicalContext &canonicalContext);

  virtual std::pair<EndType, VectorCoefficients>
  calculate(const CalculateCallback &calculateCallback = {}) override;

private:
  constexpr static const Size invalidIndex = static_cast<Size>(-1); // shit

  VectorIndices findBasis();
  Size rowBasisIndex(const Size &colIndex);

  VectorCoefficients getBasisCoefficients(const VectorIndices &basis);

  VectorCoefficients getEvaluation(const VectorCoefficients &basisCoefficients);

  Real dotProductWithCol(const VectorCoefficients &vector, const Size &index);

  EndType getEndType(const VectorCoefficients &evaluation);
  bool negativeCol(const Size &index);

  Size addToBasisRow(const Size addToBasisCol);

  void addToBasis(const Size row, const Size col);

  VectorCoefficients getAnswer();
};

#endif // SIMPLEXALGORITHM_H
