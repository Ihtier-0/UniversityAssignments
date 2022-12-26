#ifndef BASISCALCULATOR_H
#define BASISCALCULATOR_H

#include <memory>

#include "canonicalsolver.h"
#include "simplexcoefficient.h"
#include "solvecontext.h"

class BasisCalculator;
using BasisCalculatorUniquePtr = std::unique_ptr<BasisCalculator>;

// shit class
// need template CanonicalSolver but ...
class BasisCalculator {
private:
  struct Context {
    MatrixCoefficients constraintsCoefficients;
    VectorCoefficients constraintsConstants;
    SimplexCoefficientsVector objectiveFunctionCoefficients;

    static Context fromCanonical(const CanonicalContext &context);
    static CanonicalContext toCanonical(const Context &context);
  };

  BasisCalculator() = delete;
  BasisCalculator(const BasisCalculator &) = delete;
  BasisCalculator(BasisCalculator &&) = delete;
  BasisCalculator operator=(const BasisCalculator &) = delete;
  BasisCalculator operator=(BasisCalculator &&) = delete;
  BasisCalculator(const Context &context);

public:
  static BasisCalculatorUniquePtr create(const CanonicalContext &context);

  std::pair<CanonicalSolver::EndType, CanonicalContext> calculate();

private:
  VectorIndices findBasis();
  Size rowBasisIndex(const Size &colIndex);

  void completeBasis();
  void addBasisRow(const Size &rowIndex);

  SimplexCoefficientsVector getBasisCoefficients(const VectorIndices &basis);

  SimplexCoefficientsVector
  getEvaluation(const SimplexCoefficientsVector &basisCoefficients);

  SimplexCoefficient dotProductWithCol(const SimplexCoefficientsVector &vector,
                                       const Size &index);

  CanonicalSolver::EndType
  getEndType(const SimplexCoefficientsVector &evaluation);

  Size negativeAbsMax(const SimplexCoefficientsVector &coefficients);

  Size addToBasisRow(const Size addToBasisCol);

  void addToBasis(const Size row, const Size col);

  void deleteNotBasisM();

  Context mContext;
};

#endif // BASISCALCULATOR_H
