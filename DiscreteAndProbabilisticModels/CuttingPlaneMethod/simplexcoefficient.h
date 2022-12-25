#ifndef SIMPLEXCOEFFICIENT_H
#define SIMPLEXCOEFFICIENT_H

#include "globals.h"

#include <iostream>

/**
 * @brief a + b * M
 */
class SimplexCoefficient {
public:
  SimplexCoefficient(const Real &a = 0, const Real &b = 0);

  Real getA() const;
  void setA(Real A);

  Real getB() const;
  void setB(Real B);

  bool isM();

private:
  Real mA = 0;
  Real mB = 0;

  friend SimplexCoefficient operator+(const SimplexCoefficient &left,
                                      const SimplexCoefficient &right);
  friend SimplexCoefficient operator-(const SimplexCoefficient &left,
                                      const SimplexCoefficient &right);
  friend SimplexCoefficient operator*(const Real &real,
                                      const SimplexCoefficient &coefficient);
  friend SimplexCoefficient operator*(const SimplexCoefficient &coefficient,
                                      const Real &real);
  friend SimplexCoefficient operator/(const SimplexCoefficient &coefficient,
                                      const Real &real);
  friend std::ostream &operator<<(std::ostream &out,
                                  const SimplexCoefficient &coefficient);
};
using SimplexCoefficientsVector = Vector<SimplexCoefficient>;

#endif // SIMPLEXCOEFFICIENT_H
