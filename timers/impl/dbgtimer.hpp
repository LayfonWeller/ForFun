#ifndef DBGTIMER_HPP
#define DBGTIMER_HPP

#include <atomic>

template <typename timer_traits>
class dbgtimer {
   public:
    using count_t = typename timer_traits::count_t;
    using clock_select = typename timer_traits::clock_select;
    static constexpr timer_traits traits{};

    constexpr dbgtimer(clock_select cs,
                            typename timer_traits::count_t max) {}

    constexpr ~dbgtimer() = default;

    dbgtimer(const dbgtimer&) = delete;
    dbgtimer& operator=(const dbgtimer&) = delete;

    count_t get_count() const { return TCNT; }
    void inc_count(const count_t tick) { TCNT += tick; }

   private:
    std::atomic<count_t> TCNT{0};
};

#endif