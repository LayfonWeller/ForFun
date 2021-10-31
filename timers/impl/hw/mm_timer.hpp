#pragma once

#include <cstdint>

template<typename T>
concept mem_map_traits = requires(){
    {timer_traits::addr};
};

template <mem_map_traits timer_traits_>
class mm_timer {
   public:
    using timer_traits = timer_traits_;
    using count_t = typename timer_traits::count_t;
    using clock_select = typename timer_traits::clock_select;

    constexpr mm_timer(clock_select cs,
                            typename timer_traits::count_t max) {
        m_regs = new regs;
    }

    constexpr ~mm_timer() = default;
    mm_timer(const mm_timer&) = delete;
    mm_timer& operator=(const mm_timer&) = delete;
    constexpr mm_timer(mm_timer&&) = default;
    mm_timer& operator=(const mm_timer&&) = delete;

    constexpr count_t get_count() const { return m_regs->TCNT; }

   private:
    static constexpr uint8_t count_mode_a{0x03};
    static constexpr uint8_t count_mode_b{0x08};
    struct regs {
        static constexpr void* operator new(std::size_t) {
            return reinterpret_cast<void*>(timer_traits::addr);
        }
        static constexpr void operator delete(void*) {
        }
        std::uint8_t volatile TCCRA{0x33};
        std::uint8_t volatile TCCRB{0x33};
        count_t volatile TCNT;
        std::uint8_t volatile OCRA{0x33};
        std::uint8_t volatile OCRB{0x33};
    };
    regs* m_regs;
};
