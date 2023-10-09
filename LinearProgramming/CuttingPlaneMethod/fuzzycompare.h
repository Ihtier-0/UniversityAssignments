#ifndef FUZZYCOMPARE_H
#define FUZZYCOMPARE_H

#include "globals.h"

constexpr const Real defaultEps = Real(0.001);

bool fuzzyEquals(const Real &left, const Real &right,
                 const Real &eps = defaultEps);

#endif // FUZZYCOMPARE_H
