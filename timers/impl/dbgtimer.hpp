#ifndef DBGTIMER_HPP
#define DBGTIMER_HPP

#include <atomic>

template <typename timer_traits_>
class dbgtimer
{
public:
    using timer_traits = timer_traits_;
    using count_t = typename timer_traits::count_t;
    using clock_select = typename timer_traits::clock_select;

    constexpr dbgtimer(const clock_select cs,
                       const typename timer_traits::count_t max) {}

    constexpr ~dbgtimer() = default;

    constexpr dbgtimer(const dbgtimer &) = delete;
    constexpr dbgtimer &operator=(const dbgtimer &) = delete;
    constexpr dbgtimer(dbgtimer&& a) = default;
    constexpr dbgtimer& operator=(dbgtimer&& a) = default;

    constexpr count_t get_count() const { return TCNT; }
    constexpr void inc_count(const count_t tick) { TCNT += tick; }

private:
    count_t TCNT{0};
};

#endif