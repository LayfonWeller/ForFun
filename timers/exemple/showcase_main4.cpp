#include "../timers.hpp"
#include "../impl/dbgtimer.hpp"
#include "../impl/GenTimer.hpp"

struct timer0_traits
{
    using count_t = std::uint8_t;
    enum class clock_select : std::uint8_t
    {
        clk_off,
        clk_8,
        clk_16,
    };
};

static void printTimerCount(const GenTimer_te<std::uint8_t>& a) {
    printf("0x%08X \n", a.get_count());
}

int main()
{
    auto timer = dbgtimer<timer0_traits>(timer0_traits::clock_select::clk_8, 200);
    timer.inc_count(32);
    printTimerCount(timer);
    timer.inc_count(32);
    printTimerCount(timer);
    timer.inc_count(32);
    printTimerCount(timer);
    timer.inc_count(32);
    printTimerCount(timer);
    return 0;
}