#include "integerutils.h"

#include <cmath>

#include "fuzzycompare.h"

bool isInt(const Real &real) { return fuzzyEquals(real, std::floor(real)); }
