#include "basiscalculator.h"

#include "canonicalsolver.h"
#include "coefficientsutils.h"
#include "fuzzycompare.h"

#include <set>

BasisCalculator::BasisCalculator(const Context &context) : mContext(context) {}

/*static*/
BasisCalculatorUniquePtr
BasisCalculator::create(const CanonicalContext &context) {
  BasisCalculatorUniquePtr result = nullptr;

  if (!valid(context)) {
    return result;
  }

  result.reset(new BasisCalculator(Context::fromCanonical(context)));

  return result;
}

std::pair<CanonicalSolver::EndType, CanonicalContext>
BasisCalculator::calculate() {
  if (!findBasis().empty()) {
    return std::pair<CanonicalSolver::EndType, CanonicalContext>(
        CanonicalSolver::EndType::End,
        BasisCalculator::Context::toCanonical(mContext));
  }

  completeBasis();

  CanonicalSolver::EndType end;

  while (true) {
    const VectorIndices basis = findBasis();

    if (basis.empty()) {
      return std::pair<CanonicalSolver::EndType, CanonicalContext>(
          CanonicalSolver::EndType::NoEnd, {});
    }

    const SimplexCoefficientsVector basisCoefficients =
        getBasisCoefficients(basis);

    const SimplexCoefficientsVector evaluation =
        getEvaluation(basisCoefficients);

    end = getEndType(evaluation);

    if (end != CanonicalSolver::EndType::NoEnd) {
      return std::pair<CanonicalSolver::EndType, CanonicalContext>(
          end, Context::toCanonical(mContext));
    }

    const Size addToBasisCol = negativeAbsMax(evaluation);
    const Size addToBasisRow = this->addToBasisRow(addToBasisCol);

    addToBasis(addToBasisRow, addToBasisCol);

    deleteNotBasisM();
  }

  return std::pair<CanonicalSolver::EndType, CanonicalContext>(
      end, Context::toCanonical(mContext));
}

VectorIndices BasisCalculator::findBasis() {
  const Size rows = mContext.constraintsCoefficients.size();
  const Size cols = mContext.constraintsCoefficients.at(0).size();

  VectorIndices result(rows);

  Size add = 0;

  for (Size col = 0; col < cols; ++col) {
    if (Size index = rowBasisIndex(col);
        index != CanonicalSolver::invalidIndex) {
      result.at(index) = col;
      ++add;
    }
  }

  return (add != rows) ? VectorIndices() : result;
}

Size BasisCalculator::rowBasisIndex(const Size &colIndex) {
  const Size rows = mContext.constraintsCoefficients.size();

  bool findOne = false;
  bool equalOne;
  bool equalZero;

  Size index = CanonicalSolver::invalidIndex;

  for (Size row = 0; row < rows; ++row) {
    equalOne =
        fuzzyEquals(mContext.constraintsCoefficients.at(row).at(colIndex), 1);
    equalZero =
        fuzzyEquals(mContext.constraintsCoefficients.at(row).at(colIndex), 0);

    if (equalOne) {
      if (findOne) {
        return CanonicalSolver::invalidIndex;
      }

      index = row;

      findOne = true;
    }

    if (!equalOne && !equalZero) {
      return CanonicalSolver::invalidIndex;
    }
  }

  return index;
}

void BasisCalculator::completeBasis() {
  const Size rows = mContext.constraintsCoefficients.size();
  const Size cols = mContext.constraintsCoefficients.at(0).size();

  std::set<Size> basisRows;

  for (Size col = 0; col < cols; ++col) {
    if (Size index = rowBasisIndex(col);
        index != CanonicalSolver::invalidIndex) {
      basisRows.insert(index);
    }
  }

  for (Size row = 0; row < rows; ++row) {
    if (basisRows.find(row) == basisRows.end()) {
      addBasisRow(row);
    }
  }
}

void BasisCalculator::addBasisRow(const Size &rowIndex) {
  mContext.objectiveFunctionCoefficients.push_back(SimplexCoefficient(0, -1));

  for (auto &coefficient : mContext.constraintsCoefficients) {
    coefficient.push_back(0);
  }

  mContext.constraintsCoefficients[rowIndex].back() = 1;
}

SimplexCoefficientsVector
BasisCalculator::getBasisCoefficients(const VectorIndices &basis) {
  const Size nBasis = basis.size();

  SimplexCoefficientsVector basisCoefficients(nBasis);

  for (Size i = 0; i < nBasis; ++i) {
    basisCoefficients.at(i) =
        mContext.objectiveFunctionCoefficients.at(basis.at(i));
  }

  return basisCoefficients;
}

SimplexCoefficientsVector BasisCalculator::getEvaluation(
    const SimplexCoefficientsVector &basisCoefficients) {
  const Size cols = mContext.constraintsCoefficients.at(0).size();

  SimplexCoefficientsVector result;

  for (Size i = 0; i < cols; ++i) {
    result.emplace_back(dotProductWithCol(basisCoefficients, i) -
                        mContext.objectiveFunctionCoefficients.at(i));
  }

  return result;
}

SimplexCoefficient
BasisCalculator::dotProductWithCol(const SimplexCoefficientsVector &vector,
                                   const Size &index) {
  const Size rows = mContext.constraintsCoefficients.size();

  SimplexCoefficient result = 0;

  for (Size i = 0; i < rows; ++i) {
    result = result +
             vector.at(i) * mContext.constraintsCoefficients.at(i).at(index);
  }

  return result;
}

CanonicalSolver::EndType
BasisCalculator::getEndType(const SimplexCoefficientsVector &evaluation) {
  // 1
  bool allNotM = true;

  for (const auto &coefficient : mContext.objectiveFunctionCoefficients) {
    if (coefficient.isM()) {
      allNotM = false;
      break;
    }
  }

  if (allNotM) {
    return CanonicalSolver::EndType::End;
  }

  // 2
  bool allBpositive = true;

  for (const auto &coefficient : evaluation) {
    if (coefficient.getB() < 0) {
      allBpositive = false;
      break;
    }
  }

  if (allBpositive) {
    for (const auto &coefficient : mContext.objectiveFunctionCoefficients) {
      if (coefficient.isM()) {
        return CanonicalSolver::EndType::EmptySet;
      }
    }
  }

  return CanonicalSolver::EndType::NoEnd;
}

Size BasisCalculator::negativeAbsMax(
    const SimplexCoefficientsVector &coefficients) {
  const Size size = coefficients.size();

  Size max = 0;

  for (Size i = 0; i < size; ++i) {
    if (coefficients[i].getB() < 0) {
      max = i;
    }
  }

  for (Size i = 0; i < size; ++i) {
    if (coefficients[i].getB() < 0 &&
        std::abs(coefficients[i].getB()) > std::abs(coefficients[max].getB())) {
      max = i;
    }
  }

  return max;
}

Size BasisCalculator::addToBasisRow(const Size addToBasisCol) {
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

void BasisCalculator::addToBasis(const Size row, const Size col) {
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

void BasisCalculator::deleteNotBasisM() {
  const Size size = mContext.objectiveFunctionCoefficients.size();

  Size deleted = 0;

  for (Size i = 0; i < size - deleted; ++i) {
    if (mContext.objectiveFunctionCoefficients[i].isM() &&
        rowBasisIndex(i) == CanonicalSolver::invalidIndex) {
      ++deleted;

      mContext.objectiveFunctionCoefficients.erase(
          mContext.objectiveFunctionCoefficients.begin() +
          static_cast<SimplexCoefficientsVector::difference_type>(i));

      for (auto &coefficients : mContext.constraintsCoefficients) {
        coefficients.erase(
            coefficients.begin() +
            static_cast<SimplexCoefficientsVector::difference_type>(i));
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
/// BasisCalculator::Context
////////////////////////////////////////////////////////////////////////////////

BasisCalculator::Context
BasisCalculator::Context::fromCanonical(const CanonicalContext &context) {
  BasisCalculator::Context result;

  result.constraintsCoefficients = context.constraintsCoefficients;
  result.constraintsConstants = context.constraintsConstants;

  for (const auto coefficient : context.objectiveFunctionCoefficients) {
    result.objectiveFunctionCoefficients.push_back(
        SimplexCoefficient(coefficient));
  }

  return result;
}

CanonicalContext BasisCalculator::Context::toCanonical(const Context &context) {
  CanonicalContext result;

  result.constraintsCoefficients = context.constraintsCoefficients;
  result.constraintsConstants = context.constraintsConstants;

  for (const auto coefficient : context.objectiveFunctionCoefficients) {
    if (coefficient.isM()) {
      return {};
    }

    result.objectiveFunctionCoefficients.push_back(coefficient.getA());
  }

  return result;
}
