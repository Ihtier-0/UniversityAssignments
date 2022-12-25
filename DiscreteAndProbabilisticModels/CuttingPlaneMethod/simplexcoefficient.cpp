#include "simplexcoefficient.h"

#include <cassert>

#include "fuzzycompare.h"

SimplexCoefficient::SimplexCoefficient(const Real &a, const Real &b)
    : mA(a), mB(b) {}

Real SimplexCoefficient::getA() const { return mA; }

void SimplexCoefficient::setA(Real A) { mA = A; }

Real SimplexCoefficient::getB() const { return mB; }

void SimplexCoefficient::setB(Real B) { mB = B; }

bool SimplexCoefficient::isM() { return !fuzzyEquals(mB, 0); }

////////////////////////////////////////////////////////////////////////////////
/// operators
////////////////////////////////////////////////////////////////////////////////

/*friend*/
SimplexCoefficient operator+(const SimplexCoefficient &left,
                             const SimplexCoefficient &right) {
  return SimplexCoefficient(left.mA + right.mA, left.mB + right.mB);
}

/*friend*/
SimplexCoefficient operator-(const SimplexCoefficient &left,
                             const SimplexCoefficient &right) {
  return SimplexCoefficient(left.mA - right.mA, left.mB - right.mB);
}

/*friend*/
SimplexCoefficient operator*(const Real &real,
                             const SimplexCoefficient &coefficient) {
  return SimplexCoefficient(real * coefficient.mA, real * coefficient.mB);
}

/*friend*/
SimplexCoefficient operator*(const SimplexCoefficient &coefficient,
                             const Real &real) {
  return SimplexCoefficient(coefficient.mA * real, coefficient.mB * real);
}

/*friend*/
SimplexCoefficient operator/(const SimplexCoefficient &coefficient,
                             const Real &real) {
  assert(fuzzyEquals(real, 0));

  return SimplexCoefficient(coefficient.mA / real, coefficient.mB / real);
}

/*friend*/
std::ostream &operator<<(std::ostream &out,
                         const SimplexCoefficient &coefficient) {
  out << coefficient.mA << "+" << coefficient.mA << "*M";

  return out;
}
