#include "fractpart.h"

#include <cmath>

Real fractpart(const Real &real) { return real - std::floor(real); }
