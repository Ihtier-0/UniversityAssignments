#ifndef RUNGE_KUTTA_H
#define RUNGE_KUTTA_H

#include <functional>
#include <vector>

#include "types.h"

// https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods

// dy / dt = f(t, y)
// y(t[0]) = y[0]

// y[n+1] = y[n] + h / 6 * (k[1] + 2 * k[2] + 2 * k[3] + k[4])
// t[n+1] = t[n] + h

// k[1] = f(t[n]        , y[n]               )
// k[2] = f(t[n] + h / 2, y[n] + h * k[1] / 2)
// k[3] = f(t[n] + h / 2, y[n] + h * k[2] / 2)
// k[4] = f(t[n] + h    , y[n] + h * k[3]    )

template <class T, class Y, class Enable = void> class RungeKuttaSolver;

// TODO! think about change template
// R - return type
// A - arg type
// using F = std::function<R(const A&)>; etc.
// and template head and tail
// for support float -> float

template <class T, class Y>
struct RungeKuttaSolver<
    T, Y,
    //              example
    // scalar     : (float, float) -> float
    // arithmetic : (float, std::valaray<T>) -> std::valaray<T>
    std::enable_if_t<is_scalar_v<T> &&
                     (is_scalar_v<Y> || is_arithmetic_vector_v<Y>)>> {

  using F = std::function<Y(const T &, const Y &)>;
  using Point = std::pair<T, Y>;
  using Points = std::vector<Point>;

  // y' = f(t, y)
  // y(begin) = y_in_begin
  static Points solve(const F &f, const T &begin, const T &end,
                      const Y &y_in_begin, const size_t steps) {
    if (steps <= 0 || end <= begin) {
      return Points();
    }

    const T two = static_cast<T>(2);
    const T six = static_cast<T>(6);

    const T h = (end - begin) / steps;
    const T half_h = h / two;
    const T one_sixth_h = h / six;

    Points result;
    result.push_back(Point(begin, y_in_begin));

    auto step = steps;

    while (step--) {
      const auto &point = result.back();

      const T &t = point.first;
      const T t_plus_half_h = t + half_h;
      const T t_plus_h = t + h;

      const Y &y = point.second;

      const Y k1 = std::move(f(t, y));
      const Y k2 = std::move(f(t_plus_half_h, y + half_h * k1));
      const Y k3 = std::move(f(t_plus_half_h, y + half_h * k2));
      const Y k4 = std::move(f(t_plus_h, y + h * k3));

      result.emplace_back(
          Point(t_plus_h, y + one_sixth_h * (k1 + two * k2 + two * k3 + k4)));
    }

    return result;
  }
};

#endif // RUNGE_KUTTA_H
