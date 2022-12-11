#include "simplexalgorithm.h"

#include <iostream>

#include "coefficientsutils.h"
#include "fuzzycompare.h"

////////////////////////////////////////////////////////////////////////////////
/// SimplexAlgorithm new
////////////////////////////////////////////////////////////////////////////////

SimplexAlgorithm::SimplexAlgorithm(const SolveContext &solveContext)
    : mSolveContext(solveContext) {}

void SimplexAlgorithm::printCallback(const int &step,
                                     const SolveContext &solveContext) {
  std::cout << "step:" << step << std::endl;
  std::cout << "constraintsCoefficients:\n"
            << solveContext.constraintsCoefficients << std::endl;
  std::cout << "constraintsConstants:\n"
            << solveContext.constraintsConstants << std::endl;
  std::cout << "objectiveFunctionCoefficients:\n"
            << solveContext.objectiveFunctionCoefficients << std::endl;
  std::cout << std::endl;
}

SimplexAlgorithmUniquePtr
SimplexAlgorithm::create(const SolveContext &solveContext) {
  SimplexAlgorithmUniquePtr result = nullptr;

  const Size rows = solveContext.constraintsCoefficients.size();
  const Size cols = solveContext.constraintsCoefficients.at(0).size();

  if (solveContext.constraintsConstants.size() != rows) {
    return result;
  }

  if (solveContext.objectiveFunctionCoefficients.size() != cols) {
    return result;
  }

  for (const VectorCoefficients &row : solveContext.constraintsCoefficients) {
    if (row.size() != cols) {
      return result;
    }
  }

  result.reset(new SimplexAlgorithm(solveContext));

  return result;
}

VectorCoefficients
SimplexAlgorithm::calculate(const CalculateCallback &CalculateCallback) {
  int step = 0;

  EndType end;

  while (true) {
    CalculateCallback(step, mSolveContext);

    const VectorIndices basis = findBasis();

    if (basis.empty()) {
      return {};
    }

    // std::cout << "basis: " << basis << std::endl;

    const VectorCoefficients basisCoefficients = getBasisCoefficients(basis);

    // std::cout << "basisCoefficients: " << basisCoefficients << std::endl;

    const VectorCoefficients evaluation = getEvaluation(basisCoefficients);

    end = getEndType(evaluation);

    if (end != EndType::NoEnd) {
      break;
    }

    // std::cout << "evaluation: " << evaluation << std::endl;

    const Size addToBasisCol = negativeAbsMax(evaluation);
    const Size addToBasisRow = this->addToBasisRow(addToBasisCol);

    // std::cout << "addToBasisCol: " << addToBasisCol << std::endl;
    // std::cout << "addToBasisRow: " << addToBasisRow << std::endl;

    addToBasis(addToBasisRow, addToBasisCol);

    ++step;
  }

  return end == EndType::End ? getAnswer() : VectorCoefficients();
}

VectorIndices SimplexAlgorithm::findBasis() {
  const Size rows = mSolveContext.constraintsCoefficients.size();
  const Size cols = mSolveContext.constraintsCoefficients.at(0).size();

  VectorIndices result(rows);

  Size add = 0;

  for (Size col = 0; col < cols; ++col) {
    if (Size index = rowBasisIndex(col); index != invalidIndex) {
      result.at(index) = col;
      ++add;
    }
  }

  return (add != rows) ? VectorIndices() : result;
}

Size SimplexAlgorithm::rowBasisIndex(const Size &colIndex) {
  const Size rows = mSolveContext.constraintsCoefficients.size();

  bool findOne = false;
  bool equalOne;
  bool equalZero;

  Size index = invalidIndex;

  for (Size row = 0; row < rows; ++row) {
    equalOne = fuzzyEquals(
        mSolveContext.constraintsCoefficients.at(row).at(colIndex), 1);
    equalZero = fuzzyEquals(
        mSolveContext.constraintsCoefficients.at(row).at(colIndex), 0);

    if (equalOne) {
      if (findOne) {
        return invalidIndex;
      }

      index = row;

      findOne = true;
    }

    if (!equalOne && !equalZero) {
      return invalidIndex;
    }
  }

  return index;
}

VectorCoefficients
SimplexAlgorithm::getBasisCoefficients(const VectorIndices &basis) {
  const Size nBasis = basis.size();

  VectorCoefficients basisCoefficients(nBasis);

  for (Size i = 0; i < nBasis; ++i) {
    basisCoefficients.at(i) =
        mSolveContext.objectiveFunctionCoefficients.at(basis.at(i));
  }

  return basisCoefficients;
}

VectorCoefficients
SimplexAlgorithm::getEvaluation(const VectorCoefficients &basisCoefficients) {
  const Size cols = mSolveContext.constraintsCoefficients.at(0).size();

  VectorCoefficients result;

  for (Size i = 0; i < cols; ++i) {
    result.emplace_back(dotProductWithCol(basisCoefficients, i) -
                        mSolveContext.objectiveFunctionCoefficients.at(i));
  }

  return result;
}

Real SimplexAlgorithm::dotProductWithCol(const VectorCoefficients &vector,
                                         const Size &index) {
  const Size rows = mSolveContext.constraintsCoefficients.size();

  Real result = 0;

  for (Size i = 0; i < rows; ++i) {
    result +=
        vector.at(i) * mSolveContext.constraintsCoefficients.at(i).at(index);
  }

  return result;
}

SimplexAlgorithm::EndType
SimplexAlgorithm::getEndType(const VectorCoefficients &evaluation) {
  const Size size = evaluation.size();

  bool positive = true;

  for (Size i = 0; i < size && positive; ++i) {
    if (evaluation[i] < 0) {
      positive = false;
    }
  }

  if (positive) {
    return EndType::End;
  }

  for (Size i = 0; i < size; ++i) {
    if (evaluation[i] < 0 && negativeCol(i)) {
      return EndType::Infinity;
    }
  }

  return EndType::NoEnd;
}

bool SimplexAlgorithm::negativeCol(const Size &index) {
  const Size rows = mSolveContext.constraintsCoefficients.size();

  for (Size i = 0; i < rows; ++i) {
    if (mSolveContext.constraintsCoefficients.at(i).at(index) > 0) {
      return false;
    }
  }

  return true;
}

Size SimplexAlgorithm::addToBasisRow(const Size addToBasisCol) {
  const Size rows = mSolveContext.constraintsCoefficients.size();

  Size row = 0;

  Real value;

  for (Size r = 0; r < rows; ++r) {
    value = mSolveContext.constraintsCoefficients.at(r).at(addToBasisCol);

    if (value > 0) {
      row = r;
      break;
    }
  }

  for (Size r = 0; r < rows; ++r) {
    value = mSolveContext.constraintsCoefficients.at(r).at(addToBasisCol);

    if (value > 0 && mSolveContext.constraintsConstants.at(r) / value <
                         mSolveContext.constraintsConstants.at(row) /
                             mSolveContext.constraintsCoefficients.at(row).at(
                                 addToBasisCol)) {
      row = r;
    }
  }

  return row;
}

void SimplexAlgorithm::addToBasis(const Size row, const Size col) {
  const Size rows = mSolveContext.constraintsCoefficients.size();

  const Real value = mSolveContext.constraintsCoefficients.at(row).at(col);

  mSolveContext.constraintsCoefficients.at(row) =
      mSolveContext.constraintsCoefficients.at(row) / value;

  mSolveContext.constraintsConstants.at(row) /= value;

  mSolveContext.constraintsCoefficients.at(row).at(col) = 1;

  for (Size r = 0; r < rows; ++r) {
    if (r == row) {
      continue;
    }

    mSolveContext.constraintsConstants.at(r) =
        mSolveContext.constraintsConstants.at(r) -
        mSolveContext.constraintsCoefficients.at(r).at(col) *
            mSolveContext.constraintsConstants.at(row);

    mSolveContext.constraintsCoefficients.at(r) =
        mSolveContext.constraintsCoefficients.at(r) -
        mSolveContext.constraintsCoefficients.at(r).at(col) *
            mSolveContext.constraintsCoefficients.at(row);
  }

  for (Size r = 0; r < rows; ++r) {
    mSolveContext.constraintsCoefficients.at(r).at(col) = r == row ? 1 : 0;
  }
}

VectorCoefficients SimplexAlgorithm::getAnswer() {
  const Size cols = mSolveContext.constraintsCoefficients.at(0).size();

  const VectorIndices basis = findBasis();

  if (basis.empty()) {
    return {};
  }

  VectorCoefficients result(cols, 0);

  for (const Size b : basis) {
    const Size row = rowBasisIndex(b);

    result.at(b) = mSolveContext.constraintsConstants.at(row);
  }

  return result;
}
