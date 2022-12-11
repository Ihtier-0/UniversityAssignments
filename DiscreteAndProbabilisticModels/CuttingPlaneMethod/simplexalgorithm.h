#ifndef SIMPLEXALGORITHM_H
#define SIMPLEXALGORITHM_H

#include "globals.h"

#include <functional>
#include <memory>

class SimplexAlgorithm;
using SimplexAlgorithmUniquePtr = std::unique_ptr<SimplexAlgorithm>;

class SimplexAlgorithm {
public:
  struct SolveContext {
    MatrixCoefficients constraintsCoefficients;
    VectorCoefficients constraintsConstants;
    VectorCoefficients objectiveFunctionCoefficients;
  };

private:
  SimplexAlgorithm() = delete;
  SimplexAlgorithm(const SimplexAlgorithm &) = delete;
  SimplexAlgorithm(SimplexAlgorithm &&) = delete;
  SimplexAlgorithm operator=(const SimplexAlgorithm &) = delete;
  SimplexAlgorithm operator=(SimplexAlgorithm &&) = delete;

  SimplexAlgorithm(const SolveContext &solveContext);

public:
  using CalculateCallback =
      std::function<void(const int &step, const SolveContext &solveContext)>;
  static void printCallback(const int &step, const SolveContext &solveContext);

  static SimplexAlgorithmUniquePtr create(const SolveContext &solveContext);

  VectorCoefficients calculate(const CalculateCallback &calculateCallback = {});

private:
  constexpr static const Size invalidIndex = static_cast<Size>(-1); // shit

  VectorIndices findBasis();
  Size rowBasisIndex(const Size &colIndex);

  VectorCoefficients getBasisCoefficients(const VectorIndices &basis);

  VectorCoefficients getEvaluation(const VectorCoefficients &basisCoefficients);

  Real dotProductWithCol(const VectorCoefficients &vector, const Size &index);

  enum EndType { End, NoEnd, Infinity };
  EndType getEndType(const VectorCoefficients &evaluation);
  bool negativeCol(const Size &index);

  Size addToBasisRow(const Size addToBasisCol);

  void addToBasis(const Size row, const Size col);

  VectorCoefficients getAnswer();

  SolveContext mSolve;

  Real maximum;
};

#endif // SIMPLEXALGORITHM_H
