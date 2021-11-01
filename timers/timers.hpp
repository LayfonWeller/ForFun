
#ifndef TIMER_HPP
#define TIMER_HPP


#include <concepts>

/**
 * @brief Check if the type correspond to what is expected of a tiner
 * without validationt the size of the timer
 *
 * @tparam T
 */
template <typename T>
concept IsTimer = requires(const T t)
{
    {
        t.get_count()
        } -> std::same_as<typename T::count_t>;
}
&&std::is_enum_v<typename T::clock_select> &&requires(
    typename T::timer_traits t)
{
    t;
};

/**
 * @brief Concept that can validate that a timer is of a specific size
 *
 * @tparam T
 * @tparam T::count_t
 */
template <typename T, typename count = typename T::count_t>
concept IsTimer_t = std::same_as<typename T::count_t, count> && IsTimer<T>;
#endif // !TIMER_HPP