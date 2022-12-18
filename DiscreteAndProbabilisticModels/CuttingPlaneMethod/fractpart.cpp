#include "fractpart.h"

#include <cmath>

Real fractpart(const Real &real) {
  Real fractpart, intpart;
  fractpart = std::modf(real, &intpart);
  return real > 0.0f ? fractpart : 1.0f + fractpart;
}
