#include <variant>
#include <tuple>
#include <array>

namespace details
{

template<typename T>
concept is_tuple_like = requires (T t)
{
    {std::tuple_size<T>()};
};

template<typename V>
constexpr inline bool is_sequence_of_type_like = false;

template<template<typename...> typename V, typename... Vs>
constexpr inline bool is_sequence_of_type_like<V<Vs...>> = true;

template<typename T>
concept is_sequence_of_type_like_c = 
    is_sequence_of_type_like<T>;


template<typename V, typename V2>
consteval bool fits_in_impl(){
    if constexpr (is_tuple_like<V>)
    {
        if constexpr (is_tuple_like<V2>)
        {
            // TODO
            return false;
        }
        else if constexpr (std::is_bounded_array_v<V2>)
        {
            // TODO would return an X[] to fit in an tuple if first match
            using array_first_type = std::tuple_element<0, V>::type;
            return std::extent_v<V2> <= std::tuple_size<V>::value && std::same_as<array_first_type, std::remove_extent_t<V2>>;
        }
        else 
        {
            // TODO fixme : asumes that it as a value_type
            return std::same_as<typename V::value_type, V2>;
        }
    }
    return false;
}

template<typename V, typename V2>
requires(is_tuple_like<V> || is_sequence_of_type_like_c<V>)
struct fits_in : std::integral_constant<bool, 
    fits_in_impl<V, V2>()
>{};


template<template<typename...> typename V, typename V2, typename... Vs>
struct fits_in<V<Vs...>, V2> : std::integral_constant<bool, (std::same_as<Vs, V2> || ...)> {};


}


template<typename T1, typename T2>
inline constexpr bool fits_in = details::fits_in<T1, T2>::value;

template<typename T1, typename T2>
concept fits_in_c = fits_in<T2, T1>;

#if 0


using t_t = std::tuple<int, double, float>;
static_assert(fits_in<t_t, int>);
static_assert(!fits_in<t_t, char>);
// static_assert(fits_in<int, t_t>);

using v_t = std::variant<int, double, float>;
static_assert(fits_in<v_t, int>);
static_assert(!fits_in<v_t, char>);
// static_assert(fits_in<int, v_t>);

using a_t = std::array<int, 4>;
// static_assert(fits_in<a_t, int>);
static_assert(fits_in<a_t, int[4]>);
static_assert(!fits_in<a_t, int[5]>);
// static_assert(fits_in<int, a_t>);


constexpr auto test(fits_in_c<v_t> auto a)
{
    return v_t {a};
};

// constexpr static auto GLOBAL = test(char{});
constexpr static auto GLOBAL = test(int{});
#endif
