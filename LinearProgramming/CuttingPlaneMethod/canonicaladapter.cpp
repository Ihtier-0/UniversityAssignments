#include "canonicaladapter.h"

#include "coefficientsutils.h"

CanonicalAdapter::CanonicalAdapter(const SolveContext &context,
                                   CanonicalSolverUniquePtr solver)
    : mContext(context), mSolver(std::move(solver)) {}

CanonicalAdapterUniquePtr CanonicalAdapter::create(const SolveContext &context,
                                                   SolverCreator creator) {
  CanonicalAdapterUniquePtr result = nullptr;

  if (!valid(context)) {
    return result;
  }

  CanonicalSolverUniquePtr solver = creator(toCanonical(context));

  if (!solver) {
    return result;
  }

  result.reset(new CanonicalAdapter(context, std::move(solver)));

  return result;
}

std::pair<CanonicalSolver::EndType, VectorCoefficients>
CanonicalAdapter::calculate(const CalculateCallback &callback) {
  return fromCanonical(mSolver->calculate(callback));
}

static void addVariable(CanonicalContext &simplex,
                        const SolveContext &canonical, const Size &index) {
  const auto &constraint = canonical.constraints.at(index);

  if (constraint.sign == Sign::eq) {
    return;
  }

  Real value = 1;

  if (constraint.sign == Sign::ge || constraint.sign == Sign::gt) {
    value = -1;
  }

  for (auto &c : simplex.constraintsCoefficients) {
    c.push_back(0);
  }

  simplex.constraintsCoefficients.at(index).back() = value;
}

CanonicalContext CanonicalAdapter::toCanonical(const SolveContext &context) {
  CanonicalContext result;

  result.objectiveFunctionCoefficients = context.objectiveFunctionCoefficients;

  if (context.type == OptimizationType::Min) {
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

std::pair<CanonicalSolver::EndType, VectorCoefficients>
CanonicalAdapter::fromCanonical(
    const std::pair<CanonicalSolver::EndType, VectorCoefficients> &answer) {
  const auto context = answer.second;

  const Size cols = mContext.constraints.at(0).coefficients.size();

  VectorCoefficients result = context;

  result.resize(cols);

  return std::pair<CanonicalSolver::EndType, VectorCoefficients>(answer.first,
                                                                 result);
}
