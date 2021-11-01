#include <atomic>
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <memory>
#include <thread>

std::uint8_t reg[5] = {0x00, 0x10, 0x20, 0x30, 0x40};
std::uint8_t reg2[5] = {0x01, 0x01, 0x01, 0x01, 0x01};
std::uint8_t reg3[5] = {0x02, 0x02, 0x02, 0x02, 0x02};
std::uint16_t reg4[5] = {0x03, 0x13, 0x23, 0x33, 0x42};;

struct timer0_traits {
    using count_t = std::uint8_t;
    static constexpr uint8_t TIMSK{0x6E};
    static constexpr std::uint8_t* addr = reg;
    enum class clock_select : std::uint8_t {
        clk_off,
        clk_8,
        clk_16,
    };
};

struct timer1_traits {
    using count_t = std::uint8_t;
    static constexpr uint8_t TIMSK{0x6E};
    static constexpr std::uint8_t* addr = reg2;
    enum class clock_select : std::uint8_t {
        clk_off,
        clk_8,
        clk_16,
    };
};

struct timer2_traits {
    using count_t = std::uint8_t;
    static constexpr uint8_t TIMSK{0x6E};
    static constexpr std::uint8_t* addr = reg3;
    enum class clock_select : std::uint8_t {
        clk_off,
        clk_8,
        clk_16,
    };
};

struct timer3_traits {
    using count_t = std::uint16_t;
    static constexpr uint8_t TIMSK{0x6E};
    static constexpr std::uint16_t* addr = reg4;
    enum class clock_select : std::uint8_t {
        clk_off,
        clk_8,
        clk_16,
    };
};

// template <std::integral type>
// class hwtimer_t {
//    public:
//     using count_t = type;

//     template <typename timer_traits>
//     constexpr hwtimer_t(timer_traits traits,
//                         typename timer_traits::clock_select cs,
//                         typename timer_traits::count_t max)
//         : pimpl(std::make_unique<hwtimerImpl_t<timer_traits> >(cs, max)) {}

//     count_t get_count() const { return pimpl->get_count(); }

//    private:
//     struct hwTimerModel {
//         virtual ~hwTimerModel() = default;
//         virtual count_t get_count() const = 0;
//         hwTimerModel() = default;
//         hwTimerModel(const hwTimerModel&) = delete;
//         hwTimerModel& operator=(const hwTimerModel&) = delete;
//     };
//     template <typename timer_traits>
//     class hwtimerImpl_t final : public hwTimerModel {
//        public:
//         using clock_select = typename timer_traits::clock_select;
//         constexpr hwtimerImpl_t(clock_select cs,
//                                 typename timer_traits::count_t max) {}
//         // using count_t = typename timer_traits::count_t;

//         [[gnu::always_inline]] [[gnu::hot]] count_t get_count() const final {
//             // return reg.TCNT;
//             return 0;
//         }

//         ~hwtimerImpl_t() final = default;

//        private:
//         static constexpr uint8_t count_mode_a{0x03};
//         static constexpr uint8_t count_mode_b{0x08};

//         struct reg_s {
//             static constexpr void* operator new(std::size_t) {
//                 return reinterpret_cast<void*>(timer_traits::addr);
//             }
//             static constexpr void operator delete(void*) {}

//             std::uint8_t volatile TCCRA{0x33};
//             std::uint8_t volatile TCCRB{0x33};
//             std::uint8_t volatile TCNT{0x45};
//             std::uint8_t volatile OCRA{0x33};
//             std::uint8_t volatile OCRB{0x33};
//         } __attribute__((packed));

//         // reg_s& reg = *(new reg_s);
//     };

//     // std::uint8_t data[8];
//     std::unique_ptr<hwTimerModel> pimpl;
// };

template <typename timer_traits_>
class hwtimer2_impl {
   public:
    using timer_traits = timer_traits_;
    using count_t = typename timer_traits::count_t;
    using clock_select = typename timer_traits::clock_select;

    constexpr hwtimer2_impl(clock_select cs,
                            typename timer_traits::count_t max) {
        m_regs = new regs;
    }

    constexpr ~hwtimer2_impl() = default;
    hwtimer2_impl(const hwtimer2_impl&) = delete;
    hwtimer2_impl& operator=(const hwtimer2_impl&) = delete;
    constexpr hwtimer2_impl(hwtimer2_impl&&) = default;
    hwtimer2_impl& operator=(const hwtimer2_impl&&) = delete;

    constexpr count_t get_count() const { return m_regs->TCNT; }

   private:
    static constexpr uint8_t count_mode_a{0x03};
    static constexpr uint8_t count_mode_b{0x08};
    struct regs {
        static constexpr void* operator new(std::size_t) {
            return reinterpret_cast<void*>(timer_traits::addr);
        }
        static constexpr void operator delete(void*) {
            // puts(__PRETTY_FUNCTION__);
        }
        std::uint8_t volatile TCCRA{0x33};
        std::uint8_t volatile TCCRB{0x33};
        count_t volatile TCNT;
        std::uint8_t volatile OCRA{0x33};
        std::uint8_t volatile OCRB{0x33};
    };
    regs* m_regs;
};

template <typename timer_traits_>
class hwtimer_impl {
   public:
    using timer_traits = timer_traits_;
    using count_t = typename timer_traits::count_t;
    using clock_select = typename timer_traits::clock_select;

    constexpr hwtimer_impl(clock_select cs,
                           typename timer_traits::count_t max) {}

    ~hwtimer_impl() {
        // puts(__PRETTY_FUNCTION__);
    }

    hwtimer_impl(const hwtimer_impl&) = delete;
    hwtimer_impl& operator=(const hwtimer_impl&) = delete;

    count_t get_count() const { return TCNT; }

    static constexpr void* operator new(std::size_t) {
        return reinterpret_cast<void*>(timer_traits::addr);
    }
    static constexpr void operator delete(void*) {
        // puts(__PRETTY_FUNCTION__);
    }

   private:
    static constexpr uint8_t count_mode_a{0x03};
    static constexpr uint8_t count_mode_b{0x08};
    std::uint8_t volatile TCCRA{0x33};
    std::uint8_t volatile TCCRB{0x33};
    count_t volatile TCNT;
    std::uint8_t volatile OCRA{0x33};
    std::uint8_t volatile OCRB{0x33};
};

template <typename timer_traits>
class swtimer_impl {
   public:
    using count_t = typename timer_traits::count_t;
    using clock_select = typename timer_traits::clock_select;
    static constexpr timer_traits traits{};

    constexpr swtimer_impl(clock_select cs, typename timer_traits::count_t max)
        : thread([this](std::stop_token stoken) {
              while (!stoken.stop_requested()) {
                  std::this_thread::sleep_for(std::chrono::milliseconds(1));
                  this->inc_count(1);
              }
          }) {}

    constexpr ~swtimer_impl() = default;

    swtimer_impl(const swtimer_impl&) = delete;
    swtimer_impl& operator=(const swtimer_impl&) = delete;

    count_t get_count() const { return TCNT; }
    void inc_count(const count_t tick) { TCNT += tick; }

   private:
    std::atomic<count_t> TCNT{0};
    std::jthread thread;
};

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

template <typename T>
concept IsTimer = requires(T t) {
    { t.get_count() } -> std::same_as<typename T::count_t>;
};

template <typename T, typename count = typename T::count_t>
concept IsTimer_t = std::same_as<typename T::count_t, count> && IsTimer<T>;

template <std::integral type>
class GenTimer_t {
   public:
    using count_t = type;

    // template <typename timer_impl>
    // constexpr GenTimer_t(timer_impl* timer)
    //     : pimpl(/*std::unique_ptr<hwtimerImpl_t<timer_impl>>(*/
    //             new (data) hwtimerImpl_t<timer_impl>(timer)) /*)*/ {}

    template <typename timer_impl>
    requires std::is_same_v<typename timer_impl::count_t, count_t>
    constexpr GenTimer_t(timer_impl* timer)
        : pimpl(std::unique_ptr<hwtimerImpl_t<timer_impl>>(
              new hwtimerImpl_t<timer_impl>(timer))) {}

    count_t get_count() const {
        const auto querry = [this]() { return this->pimpl->get_count(); };
        return querry();
    }

   private:
    struct hwTimerModel {
        virtual ~hwTimerModel() = default;
        virtual count_t get_count() const = 0;
        hwTimerModel() = default;
        hwTimerModel(const hwTimerModel&) = delete;
        hwTimerModel& operator=(const hwTimerModel&) = delete;
    };
    template <typename timer_impl>
    struct hwtimerImpl_t final : public hwTimerModel {
        constexpr hwtimerImpl_t(timer_impl* a) : m_a(std::move(a)) {}

        virtual count_t get_count() const { return m_a->get_count(); }

        std::unique_ptr<timer_impl> m_a;
    };

    std::uint8_t data[16];
    std::unique_ptr<hwTimerModel> pimpl;
    // hwTimerModel* pimpl;
};

template <std::integral type>
class GenTimer_te {
   public:
    using count_t = type;

    template <typename timer_impl>
    requires std::is_same_v<typename timer_impl::count_t, count_t>
    constexpr GenTimer_te(const timer_impl& timer)
        // : pimpl(std::unique_ptr<TimerWrapper<timer_impl>>(
        //       new TimerWrapper<timer_impl>(timer)))
        : pimpl(std::make_unique<TimerWrapper<timer_impl>>(timer)) {
        // puts(__PRETTY_FUNCTION__);
    }

    template <typename timer_impl>
    requires std::is_same_v<typename timer_impl::count_t, count_t>
    constexpr GenTimer_te(timer_impl&& arg)
        : pimpl(std::make_unique<TimerCreated<timer_impl>>(
              std::forward<timer_impl>(arg))) {
        // puts(__PRETTY_FUNCTION__);
    }

    count_t get_count() const {
        const auto querry = [this]() { return this->pimpl->get_count(); };
        return querry();
    }

   private:
    struct TimerModel {
        constexpr virtual ~TimerModel() = default;
        constexpr virtual count_t get_count() const = 0;
        constexpr TimerModel() = default;
        constexpr TimerModel(const TimerModel&) = delete;
        constexpr TimerModel& operator=(const TimerModel&) = delete;
    };
    template <typename timer_impl>
    struct TimerCreated final : public TimerModel {
        constexpr TimerCreated(timer_impl&& timer) : m_a(std::move(timer)) {}

        constexpr virtual count_t get_count() const { return m_a.get_count(); }

        timer_impl m_a;
    };
    template <typename timer_impl>
    struct TimerWrapper final : public TimerModel {
        constexpr TimerWrapper(const timer_impl& a) : m_a(std::cref(a)) {}

        constexpr virtual count_t get_count() const {
            return m_a.get().get_count();
        }

        std::reference_wrapper<const timer_impl> m_a;
    };

    std::unique_ptr<TimerModel> pimpl;
};

// template <typename timer_impl, typename... Args>
// static constexpr GenTimer_te<typename timer_impl::timer_traits::count_t>
// create_te_timer(Args&&... args) {
//     return GenTimer_te<typename timer_impl::count_t>(
//         std::forward<Args>(args)...);
// }

template <typename timer_impl, typename... Args>
static constexpr GenTimer_te<typename timer_impl::timer_traits::count_t>
create_te_timer(Args&&... args) {
    return GenTimer_te<typename timer_impl::count_t>(
        timer_impl(std::forward<Args>(args)...));
}

static void printTimerCount(const IsTimer_t<std::uint8_t> auto& a) {
    printf("0x%02X \n", a.get_count());
}

static void printTimerCount(const IsTimer_t<std::uint16_t> auto& a) {
    printf("0x%04X \n", a.get_count());
}

static void printTimerCount(const IsTimer auto& a) {
    printf("0x%08X \n", a.get_count());
}

static auto factory(const int timer_id) {
    switch (timer_id) {
        case 3:
            return create_te_timer<hwtimer2_impl<timer0_traits>>(
                timer0_traits::clock_select::clk_8,
                static_cast<std::uint8_t>(200));

        default:
            return create_te_timer<hwtimer2_impl<timer1_traits>>(
                timer1_traits::clock_select::clk_8,
                static_cast<std::uint8_t>(200));
    }
}

int main(const int argc, const char* argv[]) {
    // hwtimer_t<std::uint8_t> timer0(timer0_traits{},
    //                                timer0_traits::clock_select::clk_8,
    //                                static_cast<std::uint8_t>(200));

    // GenTimer_t<std::uint8_t> timer0(new
    // hwtimer_impl<timer0_traits>(
    //     timer0_traits::clock_select::clk_8,
    //     static_cast<std::uint8_t>(200)));

    // printTimerCount(timer0);
    // reg[2] = 0x3d;
    // printTimerCount(timer0);
    // reg[2] = 55;
    // printTimerCount(timer0);

    // GenTimer_t<std::uint8_t> timer1(new
    // hwtimer_impl<timer1_traits>(
    //     timer1_traits::clock_select::clk_8,
    //     static_cast<std::uint8_t>(200)));

    // printTimerCount(timer1);
    // reg2[2] = 54;
    // printTimerCount(timer1);
    // reg2[2] = 35;
    // printTimerCount(timer1);

    // const auto timer = factory(argc);
    // printTimerCount(timer);

    // auto& timer2 = *(new hwtimer_impl<timer2_traits>(
    //     timer2_traits::clock_select::clk_8, static_cast<std::uint8_t>(200)));

    // TODO : Make it not take a unique_ptr
    // auto timer2_unique = std::make_unique<swtimer_impl<timer2_traits>>(
    //     timer2_traits::clock_select::clk_8, static_cast<std::uint8_t>(200));
    // auto& timer2 = *timer2_unique;

    // auto timer2_unique =
    // std::make_unique<hwtimer_impl<timer2_traits>>(timer2_traits::clock_select::clk_8,
    // static_cast<std::uint8_t>(200));
    // auto& timer2 = *timer2_unique;

    auto swtimer = dbgtimer<timer2_traits>(
        timer2_traits::clock_select::clk_8, static_cast<std::uint8_t>(200));
    printTimerCount(swtimer);

    // // auto timer2 = hwtimer2_impl<timer2_traits>(
    // //     timer2_traits::clock_select::clk_8, static_cast<std::uint8_t>(200));
    // // printTimerCount(timer2);

    // swtimer.inc_count(128);
    // // printTimerCount(swtimer);

    // printTimerCount(GenTimer_te<uint8_t>(swtimer));

    // const auto swGen = GenTimer_te<uint8_t>(swtimer);
    // printTimerCount(swGen);


    // const auto at = create_te_timer<hwtimer2_impl<timer0_traits>>(
    //     timer0_traits::clock_select::clk_8, static_cast<std::uint8_t>(200));

    // printTimerCount(at);
    // reg[2] = 0x3d;
    // printTimerCount(at);
    // reg[2] = 55;
    // printTimerCount(at);

    // reg3[2] = 54;
    // printTimerCount(timer2);
    // reg3[2] = 35;
    // printTimerCount(timer2);

    // GenTimer_t<std::uint16_t> timer3(new hwtimer_impl<timer3_traits>(
    //     timer3_traits::clock_select::clk_8,
    //     static_cast<std::uint8_t>(200)));

    // printTimerCount(timer3);
    // reg4[1] = 54;
    // printTimerCount(timer3);
    // reg4[1] = 35;
    // printTimerCount(timer3);

    //

    // printf("%X \n", timer1.get_count());

    return 0;
}