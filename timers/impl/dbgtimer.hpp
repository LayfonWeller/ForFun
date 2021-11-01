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

    constexpr dbgtimer(clock_select cs,
                       typename timer_traits::count_t max) {}

    constexpr ~dbgtimer() = default;

    dbgtimer(const dbgtimer &) = delete;
    dbgtimer &operator=(const dbgtimer &) = delete;

    count_t get_count() const { return TCNT; }
    void inc_count(const count_t tick) { TCNT += tick; }

private:
    std::atomic<count_t> TCNT{0};
};

#endif