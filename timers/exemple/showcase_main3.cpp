#include "../timers.hpp"
#include "../impl/dbgtimer.hpp"
#include "../impl/GenTimer.hpp"



struct timer0_traits {
    using count_t = std::uint8_t;
    static constexpr uint8_t TIMSK{0x6E};
    enum class clock_select : std::uint8_t {
        clk_off,
        clk_8,
        clk_16,
    };
};

struct timer1_traits {
    using count_t = std::uint8_t;
    static constexpr uint8_t TIMSK{0x6E};
    enum class clock_select : std::uint8_t {
        clk_off,
        clk_8,
        clk_16,
    };
};

static auto factory(const int timer_id) {
    switch (timer_id) {
        case 2:
            return create_te_timer<dbgtimer<timer0_traits>>(
                timer0_traits::clock_select::clk_8,
                200);

        default:
            return create_te_timer<dbgtimer<timer1_traits>>(
                timer1_traits::clock_select::clk_8,
                200);
    }
}

static void printTimerCount(const IsTimer auto& a) {
    printf("0x%08X \n", a.get_count());
}


int main(const int argc, char**) {
    const IsTimer auto timer = factory(argc);
    printTimerCount(timer);
    return 0;
}