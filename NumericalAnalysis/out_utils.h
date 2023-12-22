#ifndef OUT_UTILS_H
#define OUT_UTILS_H

#include <iostream>
#include <valarray>

template <class T>
std::ostream &operator<<(std::ostream &out, const std::valarray<T> &array) {
  const auto size = array.size();

  for (size_t i = 0; i < size; ++i) {
    out << array[i] << (i + 1 == size ? "" : ", ");
  }

  return out;
}

#endif // OUT_UTILS_H
