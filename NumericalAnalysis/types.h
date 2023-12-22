#ifndef TYPES_H
#define TYPES_H

#include <type_traits>
#include <valarray>

////////////////////////////////////////////////////////////////////////////////
/// scalar
////////////////////////////////////////////////////////////////////////////////

template <class T>
struct is_scalar : std::integral_constant<bool, std::is_arithmetic_v<T>> {};

template <class T>
inline constexpr const auto is_scalar_v = is_scalar<T>::value;

////////////////////////////////////////////////////////////////////////////////
/// arithmetic
////////////////////////////////////////////////////////////////////////////////

template <class C, class Enable = void>
struct is_arithmetic_vector : std::false_type {};

template <class C>
struct is_arithmetic_vector<
    C,
    std::enable_if_t<std::is_same_v<C, std::valarray<typename C::value_type>>>>
    : std::integral_constant<bool, is_scalar_v<typename C::value_type>> {};

template <class C>
inline constexpr const auto is_arithmetic_vector_v =
    is_arithmetic_vector<C>::value;

#endif // TYPES_H
