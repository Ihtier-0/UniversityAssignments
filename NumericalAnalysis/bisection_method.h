#ifndef BISECTION_METHOD_H
#define BISECTION_METHOD_H

#include <cmath>
#include <functional>

#include <iostream>

#include "types.h"

////////////////////////////////////////////////////////////////////////////////
/// BisectionMethodMockStepCallback
////////////////////////////////////////////////////////////////////////////////

template <class A, class R> struct BisectionMethodMockStepCallback {
  using F = std::function<R(const A &)>;

  void operator()(const size_t step, const A &x, const F &f) const {}
};

////////////////////////////////////////////////////////////////////////////////
/// BisectionMethod
////////////////////////////////////////////////////////////////////////////////

template <class A /* arg */, class R /* return */,
          class StepCallback = BisectionMethodMockStepCallback<A, R>,
          class Enable = void>
class BisectionMethod;

template <class A, class R, class StepCallback>
struct BisectionMethod<A, R, StepCallback,
                       std::enable_if_t<is_scalar_v<A> && is_scalar_v<R>>> {
  using F = std::function<R(const A &)>;

  static A solve(const F &f, const A &begin, const A &end, const R &eps,
                 const size_t steps = 1000,
                 const StepCallback &callback = StepCallback()) {
    const auto half = static_cast<A>(1) / static_cast<A>(2);
    const auto arg_eps = half * static_cast<A>(eps);

    auto l = begin;
    auto r = end;

    A center;

    A center_l;
    A center_r;

    R f_l;
    R f_r;

    size_t step = 0;

    while (step < steps && half * (r - l) > eps) {
      center = half * (r + l);

      callback(step++, center, f);

      center_l = center - arg_eps;
      center_r = center + arg_eps;

      f_l = f(center_l);
      f_r = f(center_r);

      if (f_l < f_r) {
        r = center_r;
      } else {
        l = center_l;
      }
    }

    center = half * (r + l);

    callback(step++, center, f);

    return center;
  }
};

#endif // BISECTION_METHOD_H
