#include "fractpart.h"

#include <cmath>

#include "fuzzycompare.h"

Real fractpart(const Real &real) {
  Real fractpart, intpart;
  fractpart = std::modf(real, &intpart);
  return real > 0.0f || fuzzyEquals(real, 0.0f) ? fractpart : 1.0f + fractpart;
}
