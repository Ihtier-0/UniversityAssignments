#include "gradient_descent_test.h"

#include "gradient_descent.h"
#include "out_utils.h"
#include "symmetric_derivative_gradient.h"

#include <iostream>
#include <valarray>

////////////////////////////////////////////////////////////////////////////////
/// PrintCallback
////////////////////////////////////////////////////////////////////////////////

template <class A, class R> struct PrintCallback {
  void operator()(const size_t step, const A &arg, const R &f_arg) const {
    std::cout << step << ": arg=(" << arg << ") f(arg)=" << f_arg << std::endl;
  }
};

////////////////////////////////////////////////////////////////////////////////
/// PrintCallback
////////////////////////////////////////////////////////////////////////////////

void gradient_descent_test() {
  std::cout << std::endl;
  std::cout << "/////////////////////////////////////////////////" << std::endl;
  std::cout << "/// gradient_descent_test begin" << std::endl;
  std::cout << "/////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl;

  {
    const auto f = [](const float x) {
      const auto triple_x = x * x * x;
      const auto quadruple_x = x * triple_x;

      return quadruple_x - triple_x - std::cos(5.0f * x);
    };

    const float begin = 2.0f;
    const float eps = 0.00001f;
    const float step_range = 10.0f;

    GradientDescent<float, float, SymmetricDerivativeGradient<float, float>,
                    BisectionMethod<float, float>, PrintCallback<float, float>>
        descent;
    const auto result = descent.solve(f, begin, eps, step_range);

    std::cout << std::endl;
    std::cout << "f(x)=x^4 - x^3 - cos(5.0 * x)" << std::endl;
    std::cout << "GradientDescent x=" << result.first
              << " f(x)=" << result.second << std::endl;
    std::cout << "exact x=" << 0 << " f(x)=" << -1 << std::endl;
    std::cout << std::endl;
  }

  // https://en.wikipedia.org/wiki/Test_functions_for_optimization
  {
    const auto f = [](const std::valarray<float> arg) {
      const auto x = arg[0];
      const auto y = arg[1];

      return x * x + y * y;
    };

    const std::valarray<float> begin = {1.0f, 2.0f};
    const float eps = 0.00001f;
    const float step_range = 10.0f;

    GradientDescent<std::valarray<float>, float,
                    SymmetricDerivativeGradient<std::valarray<float>, float>,
                    BisectionMethod<float, float>,
                    PrintCallback<std::valarray<float>, float>>
        descent;
    const auto result = descent.solve(f, begin, eps, step_range);

    std::cout << std::endl;
    std::cout << "Sphere function f(x, y)=x^2 + y^2" << std::endl;
    std::cout << "GradientDescent x=(" << result.first[0] << ", "
              << result.first[1] << ") f(x)=" << result.second << std::endl;
    std::cout << "exact x=(0, 0) f(x)=0" << std::endl;
    std::cout << std::endl;
  }

  {
    const auto f = [](const std::valarray<float> arg) {
      const auto x = arg[0];
      const auto y = arg[1];

      const auto l = x + 2 * y - 7;
      const auto r = 2 * x + y - 5;

      return l * l + r * r;
    };

    const std::valarray<float> begin = {0.0f, 0.0f};
    const float eps = 0.00001f;
    const float step_range = 10.0f;

    GradientDescent<std::valarray<float>, float,
                    SymmetricDerivativeGradient<std::valarray<float>, float>,
                    BisectionMethod<float, float>,
                    PrintCallback<std::valarray<float>, float>>
        descent;
    const auto result = descent.solve(f, begin, eps, step_range);

    std::cout << std::endl;
    std::cout << "Booth function f(x, y)=(x + 2 * y - 7)^2 + (2 * x + y - 5)^2"
              << std::endl;
    std::cout << "GradientDescent x=(" << result.first[0] << ", "
              << result.first[1] << ") f(x)=" << result.second << std::endl;
    std::cout << "exact x=(1, 3) f(x)=0" << std::endl;
    std::cout << std::endl;
  }

  std::cout << "/////////////////////////////////////////////////" << std::endl;
  std::cout << "/// gradient_descent_test end" << std::endl;
  std::cout << "/////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl;
}
