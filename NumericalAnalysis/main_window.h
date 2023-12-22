#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QVector>

#include <functional>
#include <valarray>

#include "qcustomplot.h"

class MainWindow : public QMainWindow {
public:
  MainWindow();

private slots:
  void compute();
  void makePlots();

private:
  // clang-format off
  const double lBegin = 0;
  const double lEnd = 180;
  const double L = 180.0;

  const double tBegin = 373.0;
  const double tEnd = 1400.0;

  const int nSteps = 3600;
  const double eps = 0.0000001f;
  const double step_range = 1.0f;

  const std::valarray<double> x_init =
    {
      1.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
    };

  const std::valarray<double> e =
    {
      25000.0,
      25000.0,
      25000.0,
      25000.0,
      40000.0,
      40000.0,
      40000.0,
      20000.0,
      20000.0,
      20000.0,
      20000.0,
      20000.0,
      20000.0,
    };

  const std::valarray<double> u =
    {
      15.190,
       8.180,
      13.198,
       3.543,
    4723.700,
     423.700,
     204.410,
       1.466 * 1e-6,
       0.013,
       0.090,
       5.428 * 1e-6,
       0.024,
       5.920 * 1e-6
    };

  const std::valarray<double> m =
    {
      18.0,
      84.0,
      56.0,
      42.0,
      28.0,
      92.0,
      16.0,
    };

  const std::valarray<double> q =
    {
      78.0,
     140.0,
     140.0,
    };

  const double g = 1750.0;
  const double gp = 3500.0;

  std::function<double(int, double)> Ri;
  std::function<double(double)> p;
  std::function<double(const std::valarray<double>&, const double)> x7;
  std::function<double(const std::valarray<double> &,
                       const double,
                       const std::valarray<double> &)> U;
  std::function<std::valarray<double>(const std::valarray<double>&,
                                      const double,
                                      const std::valarray<double>&)> f;
  std::function<double(const std::valarray<double>&)> I;
  // clang-format on

  void create_functions();

  QCustomPlot m_gasoline;
  QCustomPlot m_butene;
  QCustomPlot m_propylene;
  QCustomPlot m_ethylene;
  QCustomPlot m_by_product_1;
  QCustomPlot m_by_product_2;
  QCustomPlot m_temperature;

  const int num_params = 2;
  QVector<QDoubleSpinBox *> m_params;

  QLabel *m_functionValueLine;

  QLabel *m_profit;
  QLabel *m_check_sum;
};

#endif // MAIN_WINDOW_H
