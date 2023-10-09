#ifndef GLOBALS_H
#define GLOBALS_H

#include <cstddef>
#include <vector>

using Real = float;
using Size = size_t;
template <class T> using Vector = std::vector<T>;
template <class T> using Matrix = std::vector<std::vector<T>>;
using VectorIndices = Vector<Size>;
using VectorCoefficients = Vector<Real>;
using MatrixCoefficients = Matrix<Real>;

#endif // GLOBALS_H
