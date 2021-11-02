#include "../timers.hpp"
#include "../impl/dbgtimer.hpp"
#include "../impl/GenTimer.hpp"

enum class common_clock_select
{
    clk_off,
    clk_8,
    clk_16,
};

struct timer0_traits
{
    using count_t = std::uint8_t;
    using clock_select = common_clock_select;
};

struct timer1_traits
{
    using count_t = std::uint16_t;
    using clock_select = common_clock_select;
};

struct timer2_traits
{
    using count_t = std::uint64_t;
    using clock_select = common_clock_select;
};

static void printTimerCount(const IsTimer_t<std::uint8_t> auto &a)
{
    printf("0x%02X \n", a.get_count());
}

static void printTimerCount(const IsTimer_t<std::uint16_t> auto &a)
{
    printf("0x%04X \n", a.get_count());
}

static void printTimerCount(const IsTimer auto &a)
{
    printf("0x%08lX \n", a.get_count());
}

static void inc(IsTimer auto timer)
{
    timer.inc_count(32);
    printTimerCount(timer);
    timer.inc_count(32);
    printTimerCount(timer);
}

int main()
{
    inc(dbgtimer<timer0_traits>(common_clock_select::clk_8, 200));
    inc(dbgtimer<timer1_traits>(common_clock_select::clk_8, 2000));
    inc(dbgtimer<timer2_traits>(common_clock_select::clk_8, 20000));

    return 0;
}