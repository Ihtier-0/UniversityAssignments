#include "coefficientsutils.h"

std::string defaultVectorEnd;
std::string defaultVectorDelimiter = " ";

std::string defaultMatrixEnd;
std::string defaultMatrixDelimiter = "\n";

Size negativeAbsMax(const VectorCoefficients &coefficients) {
  const Size size = coefficients.size();

  Size max = 0;

  for (Size i = 0; i < size; ++i) {
    if (coefficients[i] < 0) {
      max = i;
    }
  }

  for (Size i = 0; i < size; ++i) {
    if (coefficients[i] < 0 &&
        std::abs(coefficients[i]) > std::abs(coefficients[max])) {
      max = i;
    }
  }

  return max;
}
