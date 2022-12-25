#include "cuttingplanemethod.h"

#include "fractpart.h"
#include "integerutils.h"

CuttingPlaneMethod::CuttingPlaneMethod(const CanonicalContext &context,
                                       SolverCreator creator)
    : CanonicalSolver(context), mCreator(creator) {}

CuttingPlaneMethod::~CuttingPlaneMethod() {}

CanonicalSolverUniquePtr
CuttingPlaneMethod::create(const CanonicalContext &context,
                           SolverCreator creator) {
  CanonicalSolverUniquePtr result = nullptr;

  if (!valid(context)) {
    return result;
  }

  result.reset(new CuttingPlaneMethod(context, creator));

  return result;
}

static void addConstraint(CanonicalContext &context, const Size &index) {
  for (VectorCoefficients &coefficients : context.constraintsCoefficients) {
    coefficients.push_back(0);
  }

  context.constraintsConstants.push_back(
      fractpart(context.constraintsConstants[index]));

  const Size size = context.constraintsCoefficients[0].size();

  VectorCoefficients coefficients(size);

  for (Size i = 0; i < size; ++i) {
    coefficients[i] = fractpart(context.constraintsCoefficients[index][i]);
  }

  coefficients.back() = -1;

  context.constraintsCoefficients.push_back(coefficients);

  context.objectiveFunctionCoefficients.push_back(0);
}

std::pair<CuttingPlaneMethod::EndType, VectorCoefficients>
CuttingPlaneMethod::calculate(const CalculateCallback &callback) {
  CanonicalContext context = mContext;

  while (true) {
    CanonicalSolverUniquePtr solve = mCreator(context);

    if (!solve) {
      return {};
    }

    std::pair<EndType, VectorCoefficients> result = solve->calculate(
        [&](const int &step, const CanonicalContext &canonicalContext) {
          callback(step, canonicalContext);

          context = canonicalContext;
        });

    if (result.first != EndType::End) {
      return result;
    }

    bool allInt = true;
    const Size size = result.second.size();
    Size index = invalidIndex;

    for (Size i = 0; allInt && i < size; ++i) {
      if (!isInt(result.second[i])) {
        allInt = false;
        index = i;
      }
    }

    if (allInt) {
      return result;
    }

    addConstraint(context, index);
  }

  return {};
}
