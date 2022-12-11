#ifndef COEFFICIENTSUTILS_H
#define COEFFICIENTSUTILS_H

#include <iostream>
#include <string>

#include "globals.h"

extern std::string defaultVectorEnd;
extern std::string defaultVectorDelimiter;

extern std::string defaultMatrixEnd;
extern std::string defaultMatrixDelimiter;

template <class T>
std::ostream &operator<<(std::ostream &out, const Vector<T> &vector) {
  const Size size = vector.size();

  for (Size i = 0; i < size; ++i) {
    out << vector.at(i)
        << ((i == size - 1) ? defaultVectorEnd : defaultVectorDelimiter);
  }

  return out;
}

template <class T>
std::ostream &operator<<(std::ostream &out, const Matrix<T> &matrix) {
  const Size size = matrix.size();

  for (Size i = 0; i < size; ++i) {
    out << matrix.at(i)
        << ((i == size - 1) ? defaultMatrixEnd : defaultMatrixDelimiter);
  }

  return out;
}

template <class T>
Vector<T> operator+(const Vector<T> &left, const Vector<T> &right) {
  const Size lsize = left.size();
  const Size rsize = right.size();

  const Size size = std::min(lsize, rsize);

  Vector<T> result(size);

  for (Size i = 0; i < size; ++i) {
    result[i] = left[i] + right[i];
  }

  return result;
}

template <class T>
Vector<T> operator*(const Real &real, const Vector<T> &vector) {
  const Size size = vector.size();

  Vector<T> result(size);

  for (Size i = 0; i < size; ++i) {
    result[i] = real * vector[i];
  }

  return result;
}

template <class T>
Vector<T> operator*(const Vector<T> &vector, const Real &real) {
  const Size size = vector.size();

  Vector<T> result(size);

  for (Size i = 0; i < size; ++i) {
    result[i] = vector[i] * real;
  }

  return result;
}

template <class T>
Vector<T> operator-(const Vector<T> &left, const Vector<T> &right) {
  return left + (-1) * right;
}

template <class T>
Vector<T> operator/(const Vector<T> &vector, const Real &real) {
  return vector * (1 / real);
}

Size negativeAbsMax(const VectorCoefficients &coefficients);

#endif // COEFFICIENTSUTILS_H
