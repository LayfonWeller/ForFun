#pragma once


template <typename timer_traits>
class dbgtimer_impl {
   public:
    using count_t = typename timer_traits::count_t;
    using clock_select = typename timer_traits::clock_select;
    static constexpr timer_traits traits{};

    constexpr dbgtimer_impl(clock_select cs,
                            typename timer_traits::count_t max) {}

    constexpr ~dbgtimer_impl() = default;

    dbgtimer_impl(const dbgtimer_impl&) = delete;
    dbgtimer_impl& operator=(const dbgtimer_impl&) = delete;

    count_t get_count() const { return TCNT; }
    void inc_count(const count_t tick) { TCNT += tick; }

   private:
    std::atomic<count_t> TCNT{0};
};