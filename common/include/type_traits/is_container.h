#pragma once

#include <type_traits>

namespace blockchain {

template <typename T, typename _ = void>
struct is_container : std::false_type {};
namespace {

template <typename... Ts> struct is_container_helper {};
} // namespace

// clang-format off
template <typename T>
struct is_container<
    T,
    std::conditional_t<
      false,
      is_container_helper<
        typename T::value_type,
        typename T::size_type,
        //itypename T::allocator_type,
        typename T::iterator,
        typename T::const_iterator,
        decltype(std::declval<T>().size()),
        decltype(std::declval<T>().begin()),
        decltype(std::declval<T>().end()),
        decltype(std::declval<T>().cbegin()),
        decltype(std::declval<T>().cend())
      >,
      void
    >
> : public std::true_type
{
};
// clang-format on
} // namespace blockchain
