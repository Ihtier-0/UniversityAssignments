#ifndef GRADIENT_DESCENT_H
#define GRADIENT_DESCENT_H

#include <functional>
#include <utility>

#include "bisection_method.h"
#include "symmetric_derivative_gradient.h"
#include "types.h"

////////////////////////////////////////////////////////////////////////////////
/// MockStepCallback
////////////////////////////////////////////////////////////////////////////////

template <class A, class R> struct GradientDescentMockStepCallback {
  void operator()(const size_t step, const A &arg, const R &f_arg) const {}
};

////////////////////////////////////////////////////////////////////////////////
/// MockStepCallback
////////////////////////////////////////////////////////////////////////////////

template <class A /* arg */, class R /* return */,
          class Gradient = SymmetricDerivativeGradient<A, R>,
          class Optimizer1D = BisectionMethod<R, R>,
          class StepCallback = GradientDescentMockStepCallback<A, R>,
          class Enable = void>
struct GradientDescent;

template <class A, class R, class Gradient, class Optimizer1D,
          class StepCallback>
struct GradientDescent<
    A, R, Gradient, Optimizer1D, StepCallback,
    std::enable_if_t<is_scalar_v<R> &&
                     (is_scalar_v<A> || is_arithmetic_vector_v<A>)>> {

  using F = std::function<R(const A &)>;
  using Extremum = std::pair<A, R>;

  GradientDescent(const Gradient &gradient = Gradient())
      : m_gradient(gradient) {}

  Extremum solve(const F &f, const A &begin, const R &eps, const R &step_range,
                 const size_t steps = 1000,
                 const StepCallback &callback = StepCallback()) {
    if (steps <= 0 || eps <= 0) {
      return Extremum();
    }

    A prev_arg;
    A current_arg = begin;

    R prev_value;
    R current_value = f(current_arg);

    size_t step = 0;

    while (step < steps) {
      callback(step++, current_arg, current_value);

      prev_arg = current_arg;
      const auto grad = m_gradient(f, prev_arg, 1e-3);

      //      const auto step_length = Optimizer1D::solve(
      //          [&](const R &step) { return f(prev_arg - step * grad); }, 0,
      //          step_range, eps, steps);

      const auto step_length = step_range;

      current_arg = prev_arg - step_length * grad;

      prev_value = current_value;
      current_value = f(current_arg);

      if (std::abs(current_value - prev_value) < eps) {
        break;
      }
    }

    callback(step++, current_arg, current_value);

    return Extremum(current_arg, current_value);
  }

private:
  Gradient m_gradient;
};

#endif // GRADIENT_DESCENT_H
