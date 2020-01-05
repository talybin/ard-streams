// Aliases and traits from C++14 and C++17
#pragma once
#include <type_traits>

namespace std
{
// C++14 features in lower versions
#if __cplusplus < 201402L
    template <bool B, class T, class F>
    using conditional_t = typename std::conditional<B, T, F>::type;

    template <bool B, class T = void>
    using enable_if_t = typename std::enable_if<B, T>::type;

    template <class T>
    using decay_t = typename std::decay<T>::type;

    template <class T>
    using remove_cv_t = typename std::remove_cv<T>::type;

    template <class T>
    using remove_const_t = typename std::remove_const<T>::type;

    template <class T>
    using remove_volatile_t = typename std::remove_volatile<T>::type;
#endif // __cplusplus < 201402L

// C++17 features in lower versions
#if __cplusplus < 201703L
    template <class...>
    using void_t = void;

    struct in_place_t {
        explicit in_place_t() = default;
    };

    template <class T> struct in_place_type_t {
        explicit in_place_type_t() = default;
    };

    template <std::size_t I> struct in_place_index_t {
        explicit in_place_index_t() = default;
    };

    template <bool B>
    using bool_constant = std::integral_constant<bool, B>;

    template <class...> struct conjunction : std::true_type { };
    template <class T1> struct conjunction<T1> : T1 { };
    template <class T1, class... Tn>
    struct conjunction<T1, Tn...>
    : conditional_t<bool(T1::value), conjunction<Tn...>, T1> { };

    template <class...> struct disjunction : std::false_type { };
    template <class T1> struct disjunction<T1> : T1 { };
    template <class T1, class... Tn>
    struct disjunction<T1, Tn...> 
    : std::conditional_t<bool(T1::value), T1, disjunction<Tn...>>  { };

    template <class T>
    struct negation : bool_constant<!bool(T::value)> { };

#ifndef __cpp_lib_is_swappable
    template <class T>
    struct is_swappable {
    private:
        template <class U, class =
            decltype(std::swap(std::declval<U&>(), std::declval<U&>()))>
        static std::true_type test(int);

        template <class>
        static std::false_type test(...);

    public:
        static constexpr bool value = decltype(test<T>(0))::value;
    };
#endif // __cpp_lib_is_swappable

#endif // __cplusplus < 201703L

} // namespace std

