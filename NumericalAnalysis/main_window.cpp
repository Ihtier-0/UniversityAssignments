#include "main_window.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include <QVector2D>
#include <QtGlobal>

#include "gradient_descent.h"
#include "runge_kutta.h"

static QVBoxLayout *plot_with_header(const QString &header, QCustomPlot *plot) {
  QVBoxLayout *layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(new QLabel(header), 0,
                    Qt::Alignment::enum_type::AlignCenter);
  layout->addWidget(plot, 1);
  return layout;
}

MainWindow::MainWindow() {
  create_functions();

  setContentsMargins(0, 0, 0, 0);

  setMinimumWidth(1280);
  setMinimumHeight(720);

  QWidget *buttons = new QWidget;
  {
    QPushButton *compute = new QPushButton("Оптимизировать");
    QObject::connect(compute, &QPushButton::clicked, this,
                     &MainWindow::compute);

    QVBoxLayout *layout = new QVBoxLayout(buttons);
    layout->setAlignment(Qt::Alignment::enum_type::AlignTop);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(compute);

    QPushButton *makePlots = new QPushButton("Построить графики");
    QObject::connect(makePlots, &QPushButton::clicked, this,
                     &MainWindow::makePlots);
    layout->addWidget(makePlots);

    m_params.resize(num_params);
    for (int i = 0; i < num_params; ++i) {
      m_params[i] = new QDoubleSpinBox();
      // m_params[i]->setRange(0, 180);
      m_params[i]->setValue(50);
      m_params[i]->setMaximum(100000);

      auto *param = new QHBoxLayout;
      param->addWidget(new QLabel(QString("Параметр %1:").arg(i + 1)));
      param->addWidget(m_params[i]);
      layout->addLayout(param);

      QObject::connect(m_params[i],
                       QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                       this, [this](double) { this->makePlots(); });
    }

    auto *profit = new QHBoxLayout;
    profit->addWidget(new QLabel("Выгода: "));
    profit->addWidget(m_profit = new QLabel("0"));
    layout->addLayout(profit);

    auto *check_sum = new QHBoxLayout;
    check_sum->addWidget(new QLabel("Контрольная сумма: "));
    check_sum->addWidget(m_check_sum = new QLabel("0"));
    layout->addLayout(check_sum);
  }

  QWidget *main = new QWidget;
  setCentralWidget(main);

  QGridLayout *layout = new QGridLayout(main);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addLayout(plot_with_header("Бензин", &m_gasoline), 0, 0);
  layout->addLayout(plot_with_header("Бутилен", &m_butene), 0, 1);
  layout->addLayout(plot_with_header("Пропилен", &m_propylene), 0, 2);
  layout->addLayout(plot_with_header("Этилен", &m_ethylene), 0, 3);

  layout->addLayout(plot_with_header("Побочные продукты 1", &m_by_product_1), 1,
                    0);
  layout->addLayout(plot_with_header("Побочные продукты 2", &m_by_product_2), 1,
                    1);
  layout->addLayout(plot_with_header("Температура", &m_temperature), 1, 2);
  layout->addWidget(buttons, 1, 3);

  m_gasoline.addGraph();
  m_butene.addGraph();
  m_propylene.addGraph();
  m_ethylene.addGraph();
  m_by_product_1.addGraph();
  m_by_product_2.addGraph();
  m_temperature.addGraph();

  m_gasoline.setAntialiasedElements(QCP::AntialiasedElement::aeAll);
  m_butene.setAntialiasedElements(QCP::AntialiasedElement::aeAll);
  m_propylene.setAntialiasedElements(QCP::AntialiasedElement::aeAll);
  m_ethylene.setAntialiasedElements(QCP::AntialiasedElement::aeAll);
  m_by_product_1.setAntialiasedElements(QCP::AntialiasedElement::aeAll);
  m_by_product_2.setAntialiasedElements(QCP::AntialiasedElement::aeAll);
  m_temperature.setAntialiasedElements(QCP::AntialiasedElement::aeAll);

  m_gasoline.setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                             QCP::iSelectPlottables);
  m_butene.setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                           QCP::iSelectPlottables);
  m_propylene.setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                              QCP::iSelectPlottables);
  m_ethylene.setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                             QCP::iSelectPlottables);
  m_by_product_1.setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                                 QCP::iSelectPlottables);
  m_by_product_2.setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                                 QCP::iSelectPlottables);
  m_temperature.setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                                QCP::iSelectPlottables);

  // up
  QVector<double> up_x = {lBegin, lEnd};
  QVector<double> up_y = {tEnd, tEnd};

  m_temperature.addGraph();
  m_temperature.graph(1)->setData(up_x, up_y);
  m_temperature.graph(1)->rescaleAxes();

  // down
  QVector<double> down_x = {lBegin, lEnd};
  QVector<double> down_y = {tBegin, tBegin};

  m_temperature.addGraph();
  m_temperature.graph(2)->setData(down_x, down_y);
  m_temperature.graph(2)->rescaleAxes();

  // left
  QVector<double> left_x = {lBegin, lBegin};
  QVector<double> left_y = {tBegin, tEnd};

  m_temperature.addGraph();
  m_temperature.graph(3)->setData(left_x, left_y);
  m_temperature.graph(3)->rescaleAxes();

  // right
  QVector<double> right_x = {lEnd, lEnd};
  QVector<double> right_y = {tBegin, tEnd};

  m_temperature.addGraph();
  m_temperature.graph(4)->setData(right_x, right_y);
  m_temperature.graph(4)->rescaleAxes();

  makePlots();
}

void MainWindow::compute() {
  std::valarray<double> params(num_params);
  for (int i = 0; i < num_params; ++i) {
    params[i] = m_params[i]->value();
  }

  GradientDescent<std::valarray<double>, double> descent;
  const auto resultParams = descent.solve(I, params, eps, step_range, 50);

  for (int i = 0; i < num_params; ++i) {
    m_params[i]->setValue(resultParams.first[i]);
    params[i] = resultParams.first[i];
  }

  makePlots();
}

void MainWindow::makePlots() {
  std::valarray<double> params(num_params);
  for (int i = 0; i < num_params; ++i) {
    params[i] = m_params[i]->value();
  }

  const RungeKuttaSolver<double, std::valarray<double>>::Points resultPoints =
      RungeKuttaSolver<double, std::valarray<double>>::solve(
          [this, &params](const double l, const std::valarray<double> &x) {
            return f(params, l, x);
          },
          0, L, x_init, nSteps);

  const auto size = resultPoints.size();

  QVector<double> x;
  x.reserve(size);
  for (int i = 0; i < size; ++i) {
    x.push_back(resultPoints[i].first);
  }

#define temp_max(_) std::max<double>(_, 0)

  QVector<double> y_gasoline;
  QVector<double> y_butene;
  QVector<double> y_propylene;
  QVector<double> y_ethylene;
  QVector<double> y_by_product_1;
  QVector<double> y_by_product_2;
  QVector<double> y_temperature;

  y_gasoline.reserve(size);
  y_butene.reserve(size);
  y_propylene.reserve(size);
  y_ethylene.reserve(size);
  y_by_product_1.reserve(size);
  y_by_product_2.reserve(size);
  y_temperature.reserve(size);

  for (int i = 0; i < size; ++i) {
    y_gasoline.push_back(temp_max(resultPoints[i].second[0]));
  }
  for (int i = 0; i < size; ++i) {
    y_butene.push_back(temp_max(resultPoints[i].second[1]));
  }
  for (int i = 0; i < size; ++i) {
    y_propylene.push_back(temp_max(resultPoints[i].second[2]));
  }
  for (int i = 0; i < size; ++i) {
    y_ethylene.push_back(temp_max(resultPoints[i].second[3]));
  }
  for (int i = 0; i < size; ++i) {
    y_by_product_1.push_back(temp_max(resultPoints[i].second[4]));
  }
  for (int i = 0; i < size; ++i) {
    y_by_product_2.push_back(temp_max(resultPoints[i].second[5]));
  }
  for (int i = 0; i < size; ++i) {
    y_temperature.push_back(x7(params, x[i]));
  }

  m_gasoline.graph(0)->setData(x, y_gasoline);
  m_butene.graph(0)->setData(x, y_butene);
  m_propylene.graph(0)->setData(x, y_propylene);
  m_ethylene.graph(0)->setData(x, y_ethylene);
  m_by_product_1.graph(0)->setData(x, y_by_product_1);
  m_by_product_2.graph(0)->setData(x, y_by_product_2);
  m_temperature.graph(0)->setData(x, y_temperature);

  m_gasoline.graph(0)->rescaleAxes();
  m_butene.graph(0)->rescaleAxes();
  m_propylene.graph(0)->rescaleAxes();
  m_ethylene.graph(0)->rescaleAxes();
  m_by_product_1.graph(0)->rescaleAxes();
  m_by_product_2.graph(0)->rescaleAxes();
  m_temperature.graph(0)->rescaleAxes();

  m_gasoline.replot();
  m_butene.replot();
  m_propylene.replot();
  m_ethylene.replot();
  m_by_product_1.replot();
  m_by_product_2.replot();
  m_temperature.replot();

  m_profit->setText(QString("%1").arg(-I(params)));

  const auto back = resultPoints.back();
  m_check_sum->setText(QString("%1").arg(back.second[0] + back.second[1] +
                                         back.second[2] + back.second[3] +
                                         back.second[4] + back.second[5]));
}

void MainWindow::create_functions() {
  Ri = [this](const int i, const double t) {
    return u[i] * std::exp(23.0 - (e[i] / t));
  };

  p = [this](const double l) { return 5.0 - l / 60.0; };

  x7 = [this](const std::valarray<double> &params, const double l) {
    //     return std::pow(l / L, params[0]) * (tEnd - tBegin) + tBegin;
    const float x1 = std::clamp(0.5, params[0], params[1]);
    const float x2 = std::clamp(params[0], params[1], L - 0.5);

    const float y1 = 1000;
    const float y2 = 500;
    if (l < x1) {
      return l * (y1 - tBegin) / (x1) + tBegin;
    }
    if (l < x2) {
      return l * (y1 - y2) / (x1 - x2) + (x1 * y2 - y1 * x2) / (x1 - x2);
    }

    return l * (tEnd - y2) / (L - x2) + (L * y2 - tEnd * x2) / (L - x2);
  };

  U = [this](const std::valarray<double> &params, const double l,
             const std::valarray<double> &x) {
    double sumMX = 0;
    for (int i = 0; i < 6; ++i) {
      sumMX += m[i + 1] * x[i];
    }

    const double t = x7(params, l);
    double res = 0;
    for (int i = 0; i < 6; ++i) {
      res += m[i + 1] * x[i] / (t * (g + gp * sumMX / m[0]));
    }

    res *= 509.209 * p(l);
    return res;
  };

  f = [this](const std::valarray<double> &params, const double l,
             const std::valarray<double> &x) -> std::valarray<double> {
    const auto UVal = U(params, l, x);
    const auto t = x7(params, l);

    const auto x1 = std::max<double>(x[0], 0);
    // const auto x1 = x[0];
    const auto x2 = x[1];
    const auto x3 = x[2];
    const auto x4 = x[3];
    const auto x5 = x[4];
    const auto x6 = x[5];

    const auto r1 = Ri(0, t);
    const auto r2 = Ri(1, t);
    const auto r3 = Ri(2, t);
    const auto r4 = Ri(3, t);
    const auto r5 = Ri(4, t);
    const auto r6 = Ri(5, t);
    const auto r7 = Ri(6, t);
    const auto r8 = Ri(7, t);
    const auto r9 = Ri(8, t);
    const auto r10 = Ri(9, t);
    const auto r11 = Ri(10, t);
    const auto r12 = Ri(11, t);
    const auto r13 = Ri(12, t);

    return {-(r1 + r2 + r3 + r4) * x1 * UVal,
            (r3 * x1 - (r6 + r7 + r10 + r13) * x2) * UVal,
            (r2 * x1 + r6 * x2 - (r5 + r9 + r12) * x3) * UVal,
            (r1 * x1 + r7 * x2 + r5 * x3 - (r8 + r11) * x4) * UVal,
            (r10 * x2 + r9 * x3 + r8 * x4) * UVal,
            (r4 * x1 + r13 * x2 + r12 * x3 + r11 * x4) * UVal};
  };

  I = [this](const std::valarray<double> &params) {
    const RungeKuttaSolver<double, std::valarray<double>>::Points result =
        RungeKuttaSolver<double, std::valarray<double>>::solve(
            [this, &params](const double l, const std::valarray<double> &x) {
              return f(params, l, x);
            },
            0, L, x_init, nSteps);

    std::valarray<double> lastResult = result.back().second;
    if (lastResult[0] < 0) {
      lastResult[0] = 0;
    }
    double chislitel = 0;
    for (int i = 1; i <= 3; ++i) {
      chislitel += q[i - 1] * m[i + 1] * lastResult[i];
    }

    double znamenatel = 0;
    for (int i = 0; i < 6; ++i) {
      znamenatel += m[i + 1] * lastResult[i];
    }

    return -chislitel / znamenatel;
  };
}
