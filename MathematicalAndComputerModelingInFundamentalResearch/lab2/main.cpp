#include <iostream>
#include <string>
#include <vector>

// clang-format off

int main(int argc, char *argv[]) {
  constexpr const double a = 15; // мм
  constexpr const double b = 30; // мм

  constexpr const double qb = 4000; // МПа
  constexpr const double qa = 0;    // МПа

  std::cout << "a: "  << a  << std::endl;
  std::cout << "b: "  << b  << std::endl;
  std::cout << "qb: " << qb << std::endl;
  std::cout << "qa: " << qa << std::endl;
  std::cout << std::endl;

  // from ansys
  constexpr const double E = 2e+5; // МПА Модуль Юнга
  constexpr const double v = 0.3;  // Коэффициент Пуассона
  // K = 1.6667e+11; Па Модуль всестороннего сжатия
  // m = 7.6923e+10; Па Модуль сдвига

  constexpr const double l =       (v * E)          // МПа первый параметр Ламе
                         / //-----------------------
                             ((1 + v) * (1 - 2 * v));

  constexpr const double m =       E       // MПа
                         / //-------------
                             (2 * (1 + v));

  constexpr const double k =         E         // MПа
                         / //-----------------
                             (3 * (1 - 2 * v));

  std::cout << "E: " << E << std::endl;
  std::cout << "v: " << v << std::endl;
  std::cout << "l: " << l << std::endl;
  std::cout << "m: " << m << std::endl;
  std::cout << "k: " << k << std::endl;
  std::cout << std::endl;

  constexpr const double step = 0.1;

  constexpr const int nSteps = (b - a) / step;

  std::vector<double> radius = {
    0.0000 + a,
    0.3125 + a,
    0.6250 + a,
    0.9375 + a,
    1.2500 + a,
    1.5625 + a,
    1.8750 + a,
    2.1875 + a,
    2.5000 + a,
    2.8125 + a,
    3.1250 + a,
    3.4375 + a,
    3.7500 + a,
    4.0625 + a,
    4.3750 + a,
    4.6875 + a,
    5.0000 + a,
    5.3125 + a,
    5.6250 + a,
    5.9375 + a,
    6.2500 + a,
    6.5625 + a,
    6.8750 + a,
    7.1875 + a,
    7.5000 + a,
    7.8125 + a,
    8.1250 + a,
    8.4375 + a,
    8.7500 + a,
    9.0625 + a,
    9.3750 + a,
    9.6875 + a,
    10.000 + a,
    10.313 + a,
    10.625 + a,
    10.938 + a,
    11.250 + a,
    11.563 + a,
    11.875 + a,
    12.188 + a,
    12.500 + a,
    12.813 + a,
    13.125 + a,
    13.437 + a,
    13.750 + a,
    14.062 + a,
    14.375 + a,
    14.687 + a,
    15.000 + a,
  };

  // перемещение
  std::cout << "u (directional deformation): " << std::endl;
  for(const auto r : radius)
  {
    const double u_first = (r * (qb * b * b - qa * a * a))
                       / //-------------------------------
                           (2 * (l + m) * (a * a - b * b));

    const double u_second =  ((qb - qa) * a * a * b * b)
                        / //-----------------------------
                            (r * 2 * m * (a * a - b * b));

    const double u = u_first + u_second;

    std::cout << '(' << r << ", " << u << ")," << std::endl;
  }
  std::cout << std::endl << std::endl;

  // радиальная деформация
  std::cout << "er (maximum principal elastic strain): " << std::endl;
  for(const auto r : radius)
  {
    const double er_first =    (qb * b * b - qa * a * a)
                        / //-------------------------------
                            (2 * (l + m) * (a * a - b * b));

    const double er_second =    ((qb - qa) * a * a * b * b)
                         / //---------------------------------
                             (r * r * 2 * m * (a * a - b * b));

    const double er = er_first - er_second;

    std::cout << '(' << r << ", " << er << ")," << std::endl;
  }
  std::cout << std::endl << std::endl;

  // окружная деформация
  std::cout << "et (minimum principal elastic strain): " << std::endl;
  for(const auto r : radius)
  {
    const double et_first =    (qb * b * b - qa * a * a)
                        / //-------------------------------
                            (2 * (l + m) * (a * a - b * b));

    const double et_second =    ((qb - qa) * a * a * b * b)
                         / //---------------------------------
                             (r * r * 2 * m * (a * a - b * b));

    const double et = et_first + et_second;

    std::cout << '(' << r << ", " << et << ")," << std::endl;
  }
  std::cout << std::endl << std::endl;

  // радиальное напряжение
  std::cout << "sr (maximum principal stress): " << std::endl;
  for(const auto r : radius)
  {
    const double sr_first = (qa * a * a * (r * r - b * b))
                        / //------------------------------
                               (r * r * (b * b - a * a));

    const double sr_second = (qb * b * b * (a * a - r * r))
                         / //------------------------------
                               (r * r * (b * b - a * a));

    const double sr = sr_first + sr_second;

    std::cout << '(' << r << ", " << sr << ")," << std::endl;
  }
  std::cout << std::endl << std::endl;

  // окружное напряжение
  std::cout << "st (minimum principal stress): " << std::endl;
  for(const auto r : radius)
  {
    const double st_first = (qa * a * a * (r * r + b * b))
                        / //------------------------------
                               (r * r * (b * b - a * a));

    const double st_second = (qb * b * b * (a * a + r * r))
                         / //------------------------------
                               (r * r * (b * b - a * a));

    const double st = st_first - st_second;

    std::cout << '(' << r << ", " << st << ")," << std::endl;
  }

  return 0;
}

// clang-format on
