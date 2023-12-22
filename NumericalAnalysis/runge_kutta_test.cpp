#include "runge_kutta_test.h"

#include "runge_kutta.h"

#include <iomanip>
#include <iostream>
#include <valarray>

void runge_kutta_test() {
  std::cout << std::endl;
  std::cout << "/////////////////////////////////////////////////" << std::endl;
  std::cout << "/// runge_kutta_test begin" << std::endl;
  std::cout << "/////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl;

  const auto width = 15;
  const auto setw = std::setw(width);
  const auto setw2d = std::setw(2 * width + 4);

  const auto precision = 6;
  const auto setprecision = std::setprecision(precision);

  {
    std::cout << "dy/dt=1 / (t + 2 * y)" << std::endl;
    std::cout << "y(0)=-1" << std::endl;
    std::cout << "y(t)=-1 - 0.5 * t" << std::endl;
    std::cout << std::endl;

    const auto f = [](const float t, const float y) -> float {
      return 1 / (t + 2 * y);
    };

    const float begin = 0.0f;
    const float end = 1.0f;
    const float y_in_begin = -1.0f;
    const int steps = 10;

    const auto result =
        RungeKuttaSolver<float, float>::solve(f, begin, end, y_in_begin, steps);

    const auto exact = [](const float t) { return -1.0f - 0.5f * t; };

    std::cout << setw << "t" << ' ' << setw << "y_runge_kutta" << ' ' << setw
              << "y_exact" << std::endl;
    for (const auto &r : result) {
      std::cout << setw << setprecision << r.first << ' ' << setw
                << setprecision << r.second << ' ' << setw << setprecision
                << exact(r.first) << std::endl;
    }

    std::cout << std::endl;
  }

  {
    std::cout << "dy[0]/dt=-2 * y[0] + 4 * y[1]" << std::endl;
    std::cout << "dy[1]/dt=    -y[0] + 3 * y[1]" << std::endl;
    std::cout << "y[0](0)=3" << std::endl;
    std::cout << "y[1](0)=0" << std::endl;
    std::cout << "y[0](t)=4 * e^(-t) - e^(2 * t)" << std::endl;
    std::cout << "y[1](t)=    e^(-t) - e^(2 * t)" << std::endl;
    std::cout << std::endl;

    const auto f = [](const float t,
                      const std::valarray<float> y) -> std::valarray<float> {
      const auto y_0 = y[0];
      const auto y_1 = y[1];

      return {-2 * y_0 + 4 * y_1, -y_0 + 3 * y_1};
    };

    const float begin = 0.0f;
    const float end = 1.0f;
    const std::valarray<float> y_in_begin = {3.0f, 0.0f};
    const int steps = 10;

    const auto result = RungeKuttaSolver<float, std::valarray<float>>::solve(
        f, begin, end, y_in_begin, steps);

    const auto exact = [](const float t) -> std::valarray<float> {
      return {4 * std::exp(-t) - std::exp(2 * t),
              std::exp(-t) - std::exp(2 * t)};
    };

    std::cout << setw << "t" << ' ' << setw2d << "y_runge_kutta" << ' '
              << setw2d << "y_exact" << std::endl;
    for (const auto &r : result) {
      const auto exct = exact(r.first);

      std::cout << setw << setprecision << r.first << " (" << setw
                << setprecision << r.second[0] << ", " << setw << setprecision
                << r.second[1] << ") "
                << " (" << setw << setprecision << exct[0] << ", " << setw
                << setprecision << exct[1] << ") " << std::endl;
    }

    std::cout << std::endl;
  }

  {
    std::cout << "dy[0]/dt=(1 / t) * y[0]" << std::endl;
    std::cout << "dy[1]/dt=t * y[0] - 2 * y[1]" << std::endl;
    std::cout << "y[0](1)=1" << std::endl;
    std::cout << "y[1](1)=4" << std::endl;
    std::cout << "y[0](t)=t" << std::endl;
    std::cout
        << "y[1](t)=(4 - 1 / 4) * e^(2 - 2 * t) + (t^2 / 2 - t / 2 + 1 / 4)"
        << std::endl;
    std::cout << std::endl;

    const auto f = [](const float t,
                      const std::valarray<float> y) -> std::valarray<float> {
      const auto y_0 = y[0];
      const auto y_1 = y[1];

      return {(1 / t) * y_0, t * y_0 - 2 * y_1};
    };

    const float begin = 1.0f;
    const float end = 2.0f;
    const std::valarray<float> y_in_begin = {1.0f, 4.0f};
    const int steps = 10;

    const auto result = RungeKuttaSolver<float, std::valarray<float>>::solve(
        f, begin, end, y_in_begin, steps);

    const auto exact = [](const float t) -> std::valarray<float> {
      return {t, (4 - 1.0f / 4) * std::exp(2.0f - 2.0f * t) +
                     (t * t / 2 - t / 2 + 1.0f / 4)};
    };

    std::cout << setw << "t" << ' ' << setw2d << "y_runge_kutta" << ' '
              << setw2d << "y_exact" << std::endl;
    for (const auto &r : result) {
      const auto exct = exact(r.first);

      std::cout << setw << setprecision << r.first << " (" << setw
                << setprecision << r.second[0] << ", " << setw << setprecision
                << r.second[1] << ") "
                << " (" << setw << setprecision << exct[0] << ", " << setw
                << setprecision << exct[1] << ") " << std::endl;
    }

    std::cout << std::endl;
  }

  std::cout << "/////////////////////////////////////////////////" << std::endl;
  std::cout << "/// runge_kutta_test end" << std::endl;
  std::cout << "/////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl;
}
