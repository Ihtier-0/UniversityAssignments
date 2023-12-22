#ifndef SYMMETRIC_DERIVATIVE_GRADIENT_H
#define SYMMETRIC_DERIVATIVE_GRADIENT_H

#include <functional>

#include "types.h"

// https://en.wikipedia.org/wiki/Symmetric_derivative

template <class A /* arg */, class R /* return */, class Enable = void>
struct SymmetricDerivativeGradient;

template <class A, class R>
struct SymmetricDerivativeGradient<
    A, R, std::enable_if_t<is_scalar_v<A> && is_scalar_v<R>>> {

  using F = std::function<R(const A &)>;

  A operator()(const F &f, const A &x, const R &eps) {
    const auto double_eps = 2 * eps;
    return (f(x + eps) - f(x - eps)) / double_eps;
  };
};

template <class A, class R>
struct SymmetricDerivativeGradient<
    A, R, std::enable_if_t<is_arithmetic_vector_v<A> && is_scalar_v<R>>> {

  using F = std::function<R(const A &)>;

  A operator()(const F &f, const A &x, const R &eps) {
    const auto double_eps = 2 * eps;
    const auto size = std::size(x);

    A l_arg = x;
    A r_arg = x;

    A res(size);

    for (int i = 0; i < size; ++i) {
      l_arg[i] += eps;
      r_arg[i] -= eps;

      res[i] = (f(l_arg) - f(r_arg)) / double_eps;

      l_arg[i] = r_arg[i] = x[i];
    }

    return res;
  };
};

#endif // SYMMETRIC_DERIVATIVE_GRADIENT_H
