#include "fuzzycompare.h"

#include <cmath>

bool fuzzyEquals(const Real &left, const Real &right, const Real &eps) {
  return std::abs(left - right) < eps;
}
