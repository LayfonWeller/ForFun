// https://godbolt.org/z/G716Kqn1e

#include "../timers.hpp"
#include "../impl/hw/mm_timer.hpp"

std::uint8_t reg[5] = {0x00, 0x10, 0x20, 0x30, 0x40};

struct timer0_traits
{
    using count_t = std::uint8_t;
    static constexpr uint8_t TIMSK{0x6E};
    static constexpr std::uint8_t *addr = reg;
    enum class clock_select : std::uint8_t
    {
        clk_off,
        clk_8,
        clk_16,
    };
};

int main()
{
    const IsTimer auto timer =
        mm_timer<timer0_traits>(timer0_traits::clock_select::clk_8, 200);
    return timer.get_count();
}