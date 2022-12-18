#include "simplexalgorithm.h"

#include <iostream>

#include "coefficientsutils.h"
#include "fuzzycompare.h"

SimplexAlgorithm::SimplexAlgorithm(const CanonicalContext &context)
    : CanonicalSolver(context) {}

SimplexAlgorithm::~SimplexAlgorithm() {}

CanonicalSolverUniquePtr
SimplexAlgorithm::create(const CanonicalContext &context) {
  CanonicalSolverUniquePtr result = nullptr;

  if (!valid(context)) {
    return result;
  }

  result.reset(new SimplexAlgorithm(context));

  return result;
}

VectorCoefficients
SimplexAlgorithm::calculate(const CalculateCallback &CalculateCallback) {
  int step = 0;

  EndType end;

  while (true) {
    CalculateCallback(step, mContext);

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
  const Size rows = mContext.constraintsCoefficients.size();
  const Size cols = mContext.constraintsCoefficients.at(0).size();

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
  const Size rows = mContext.constraintsCoefficients.size();

  bool findOne = false;
  bool equalOne;
  bool equalZero;

  Size index = invalidIndex;

  for (Size row = 0; row < rows; ++row) {
    equalOne =
        fuzzyEquals(mContext.constraintsCoefficients.at(row).at(colIndex), 1);
    equalZero =
        fuzzyEquals(mContext.constraintsCoefficients.at(row).at(colIndex), 0);

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
        mContext.objectiveFunctionCoefficients.at(basis.at(i));
  }

  return basisCoefficients;
}

VectorCoefficients
SimplexAlgorithm::getEvaluation(const VectorCoefficients &basisCoefficients) {
  const Size cols = mContext.constraintsCoefficients.at(0).size();

  VectorCoefficients result;

  for (Size i = 0; i < cols; ++i) {
    result.emplace_back(dotProductWithCol(basisCoefficients, i) -
                        mContext.objectiveFunctionCoefficients.at(i));
  }

  return result;
}

Real SimplexAlgorithm::dotProductWithCol(const VectorCoefficients &vector,
                                         const Size &index) {
  const Size rows = mContext.constraintsCoefficients.size();

  Real result = 0;

  for (Size i = 0; i < rows; ++i) {
    result += vector.at(i) * mContext.constraintsCoefficients.at(i).at(index);
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
  const Size rows = mContext.constraintsCoefficients.size();

  for (Size i = 0; i < rows; ++i) {
    if (mContext.constraintsCoefficients.at(i).at(index) > 0) {
      return false;
    }
  }

  return true;
}

Size SimplexAlgorithm::addToBasisRow(const Size addToBasisCol) {
  const Size rows = mContext.constraintsCoefficients.size();

  Size row = 0;

  Real value;

  for (Size r = 0; r < rows; ++r) {
    value = mContext.constraintsCoefficients.at(r).at(addToBasisCol);

    if (value > 0) {
      row = r;
      break;
    }
  }

  for (Size r = 0; r < rows; ++r) {
    value = mContext.constraintsCoefficients.at(r).at(addToBasisCol);

    if (value > 0 &&
        mContext.constraintsConstants.at(r) / value <
            mContext.constraintsConstants.at(row) /
                mContext.constraintsCoefficients.at(row).at(addToBasisCol)) {
      row = r;
    }
  }

  return row;
}

void SimplexAlgorithm::addToBasis(const Size row, const Size col) {
  const Size rows = mContext.constraintsCoefficients.size();

  const Real value = mContext.constraintsCoefficients.at(row).at(col);

  mContext.constraintsCoefficients.at(row) =
      mContext.constraintsCoefficients.at(row) / value;

  mContext.constraintsConstants.at(row) /= value;

  mContext.constraintsCoefficients.at(row).at(col) = 1;

  for (Size r = 0; r < rows; ++r) {
    if (r == row) {
      continue;
    }

    mContext.constraintsConstants.at(r) =
        mContext.constraintsConstants.at(r) -
        mContext.constraintsCoefficients.at(r).at(col) *
            mContext.constraintsConstants.at(row);

    mContext.constraintsCoefficients.at(r) =
        mContext.constraintsCoefficients.at(r) -
        mContext.constraintsCoefficients.at(r).at(col) *
            mContext.constraintsCoefficients.at(row);
  }

  for (Size r = 0; r < rows; ++r) {
    mContext.constraintsCoefficients.at(r).at(col) = r == row ? 1 : 0;
  }
}

VectorCoefficients SimplexAlgorithm::getAnswer() {
  const Size cols = mContext.constraintsCoefficients.at(0).size();

  const VectorIndices basis = findBasis();

  if (basis.empty()) {
    return {};
  }

  VectorCoefficients result(cols, 0);

  for (const Size b : basis) {
    const Size row = rowBasisIndex(b);

    result.at(b) = mContext.constraintsConstants.at(row);
  }

  return result;
}
