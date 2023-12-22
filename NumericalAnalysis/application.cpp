#include "application.h"

#include "bisection_method_test.h"
#include "gradient_descent_test.h"
#include "runge_kutta_test.h"

#include "main_window.h"

#include <QApplication>

#include <cmath>
#include <cstdlib>
#include <valarray>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
/// tests_application
////////////////////////////////////////////////////////////////////////////////

int tests_application() {
  bisection_method_test();
  gradient_descent_test();
  runge_kutta_test();
  return EXIT_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// gui_application
////////////////////////////////////////////////////////////////////////////////

int gui_application() {
  MainWindow main;
  main.show();
  return qApp->exec();
}
