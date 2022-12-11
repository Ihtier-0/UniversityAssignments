#include "canonicaladapter.h"

#include "coefficientsutils.h"

CanonicalAdapter::CanonicalAdapter(const SolveContext &context,
                                   SimplexAlgorithmUniquePtr simplex)
    : mContext(context), mSimplex(std::move(simplex)) {}

CanonicalAdapterUniquePtr
CanonicalAdapter::create(const SolveContext &context) {
  CanonicalAdapterUniquePtr result = nullptr;

  if (context.type == OptimizationType::Unknown) {
    return result;
  }

  const Size cols = context.constraints.at(0).coefficients.size();

  if (context.objectiveFunctionCoefficients.size() != cols) {
    return result;
  }

  for (const Constraint &c : context.constraints) {
    if (c.sign == Sign::Unknown || c.coefficients.size() != cols) {
      return result;
    }
  }

  SimplexAlgorithmUniquePtr simplex =
      SimplexAlgorithm::create(toCanonical(context));

  if (!simplex) {
    return result;
  }

  result.reset(new CanonicalAdapter(context, std::move(simplex)));

  return result;
}

VectorCoefficients CanonicalAdapter::calculate(
    const SimplexAlgorithm::CalculateCallback &callback) {
  return fromCanonical(mSimplex->calculate(callback));
}

static void addVariable(SimplexAlgorithm::SolveContext &simplex,
                        const CanonicalAdapter::SolveContext &canonical,
                        const Size &index) {
  const auto &constraint = canonical.constraints.at(index);

  if (constraint.sign == CanonicalAdapter::Sign::eq) {
    return;
  }

  Real value = 1;

  if (constraint.sign == CanonicalAdapter::Sign::ge ||
      constraint.sign == CanonicalAdapter::Sign::gt) {
    value = -1;
  }

  for (auto &c : simplex.constraintsCoefficients) {
    c.push_back(0);
  }

  simplex.constraintsCoefficients.at(index).back() = value;
}

SimplexAlgorithm::SolveContext
CanonicalAdapter::toCanonical(const CanonicalAdapter::SolveContext &context) {
  SimplexAlgorithm::SolveContext result;

  result.objectiveFunctionCoefficients = context.objectiveFunctionCoefficients;

  if (context.type == CanonicalAdapter::OptimizationType::Min) {
    result.objectiveFunctionCoefficients =
        -1.0f * result.objectiveFunctionCoefficients;
  }

  for (const auto &c : context.constraints) {
    result.constraintsCoefficients.push_back(c.coefficients);

    result.constraintsConstants.push_back(c.constants);
  }

  const Size size = context.constraints.size();

  for (Size i = 0; i < size; ++i) {
    if (context.constraints.at(i).sign != Sign::eq) {
      addVariable(result, context, i);
      result.objectiveFunctionCoefficients.push_back(0);
    }
  }

  return result;
}

VectorCoefficients
CanonicalAdapter::fromCanonical(const VectorCoefficients &context) {
  const Size cols = mContext.constraints.at(0).coefficients.size();

  VectorCoefficients result = context;

  result.resize(cols);

  return result;
}
