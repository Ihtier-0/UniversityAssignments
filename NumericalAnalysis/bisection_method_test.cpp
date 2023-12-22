#include "bisection_method_test.h"

#include <iostream>

#include "bisection_method.h"
#include "out_utils.h"

////////////////////////////////////////////////////////////////////////////////
/// PrintCallback
////////////////////////////////////////////////////////////////////////////////

template <class A, class R> struct PrintCallback {
  using F = std::function<R(const A &)>;

  void operator()(const size_t step, const A &x, const F &f) const {
    std::cout << step << ": x=" << x << " f(x)=" << f(x) << std::endl;
  }
};

////////////////////////////////////////////////////////////////////////////////
/// bisection_method_test
////////////////////////////////////////////////////////////////////////////////

void bisection_method_test() {
  std::cout << std::endl;
  std::cout << "/////////////////////////////////////////////////" << std::endl;
  std::cout << "/// bisection_method_test begin" << std::endl;
  std::cout << "/////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl;

  const auto f = [](const float x) { return std::sin(2.0f * x); };
  const float begin = M_PI;
  const float end = 2.0f * M_PI;
  const float eps = 0.0001f;

  const auto x =
      BisectionMethod<float, float, PrintCallback<float, float>>::solve(
          f, begin, end, eps);
  const auto exact_x = 7.0f * M_PI / 4.0f;

  std::cout << std::endl;
  std::cout << "f(x)=sin(2.0 * x), x<-[Pi, 2.0 * Pi]" << std::endl;
  std::cout << "BisectionMethod x=" << x << " f(x)=" << f(x) << std::endl;
  std::cout << "exact x=" << exact_x << " f(x)=" << f(exact_x) << std::endl;
  std::cout << std::endl;
  std::cout << "/////////////////////////////////////////////////" << std::endl;
  std::cout << "/// bisection_method_test end" << std::endl;
  std::cout << "/////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl;
}
