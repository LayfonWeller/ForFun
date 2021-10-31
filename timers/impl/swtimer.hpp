#pragma once

#include <thread>
#include <atomic>

/**
 * @brief Quick
 *
 * @tparam timer_traits
 */
template <typename timer_traits_t>
class swtimer_impl
{
public:
    using timer_traits = typename timer_traits;
    using count_t = typename timer_traits::count_t;
    using clock_select = typename timer_traits::clock_select;

    constexpr swtimer_impl(clock_select cs, typename timer_traits::count_t max)
        : thread([this](std::stop_token stoken)
                 {
                     while (!stoken.stop_requested())
                     {
                         std::this_thread::sleep_for(std::chrono::milliseconds(1));
                         TCNT += tick;
                     }
                 })
    {
    }

    constexpr ~swtimer_impl() = default;

    swtimer_impl(const swtimer_impl &) = delete;
    swtimer_impl &operator=(const swtimer_impl &) = delete;

    count_t get_count() const { return TCNT; }

private:
    std::atomic<count_t> TCNT{0};
    std::jthread thread;
};