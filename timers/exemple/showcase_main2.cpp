#include "../timers.hpp"
#include "../impl/dgbtimer.hpp"
#include "../impl/GenTimer.hpp"

struct timer0_traits
{
    using count_t = std::uint8_t;
    static constexpr uint8_t TIMSK{0x6E};
    enum class clock_select : std::uint8_t
    {
        clk_off,
        clk_8,
        clk_16,
    };
};

int main()
{
    const GenTimer_te<std::uint8_t> timer =
        create_te_timer<dbgtimer<timer0_traits>>(timer0_traits::clock_select::clk_8, 200);
    return timer.get_count();
}