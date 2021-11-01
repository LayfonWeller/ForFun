#ifndef GEN_TIMER_HPP
#define GEN_TIMER_HPP

#include <concepts>
#include <utility>

template <std::integral type>
class GenTimer_te
{
public:
    using count_t = type;

    template <typename timer_impl>
    requires std::is_same_v<typename timer_impl::count_t, count_t>
    constexpr GenTimer_te(const timer_impl &timer)

        : pimpl(std::make_unique<TimerWrapper<timer_impl>>(timer))
    {
    }

    template <typename timer_impl>
    requires std::is_same_v<typename timer_impl::count_t, count_t>
    constexpr GenTimer_te(timer_impl &&arg)
        : pimpl(std::make_unique<TimerCreated<timer_impl>>(
              std::forward<timer_impl>(arg)))
    {
    }

    count_t get_count() const
    {
        const auto querry = [this]()
        { return this->pimpl->get_count(); };
        return querry();
    }

private:
    struct TimerModel
    {
        constexpr virtual ~TimerModel() = default;
        constexpr virtual count_t get_count() const = 0;
        constexpr TimerModel() = default;
        constexpr TimerModel(const TimerModel &) = delete;
        constexpr TimerModel &operator=(const TimerModel &) = delete;
    };
    template <typename timer_impl>
    struct TimerCreated final : public TimerModel
    {
        constexpr TimerCreated(timer_impl &&timer) : m_a(std::move(timer)) {}

        constexpr virtual count_t get_count() const { return m_a.get_count(); }

        timer_impl m_a;
    };
    template <typename timer_impl>
    struct TimerWrapper final : public TimerModel
    {
        constexpr TimerWrapper(const timer_impl &a) : m_a(std::cref(a)) {}

        constexpr virtual count_t get_count() const
        {
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
constexpr GenTimer_te<typename timer_impl::timer_traits::count_t>
create_te_timer(Args &&...args)
{
    return GenTimer_te<typename timer_impl::count_t>(
        timer_impl(std::forward<Args>(args)...));
}

#endif