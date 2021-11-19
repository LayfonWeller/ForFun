#include <algorithm>
#include <array>
#include <concepts>
#include <cstdint>
#include <iterator>
#include <numeric>
#include <optional>

#include "../utilities/PrintNum.hpp"
#include <vector>

template <typename T>
[[gnu::const, nodiscard]]
constexpr std::optional<T> safe_add(T a, T b) {
    if (a > 0 && b > std::numeric_limits<T>::max() - a) {
        return {};
    } else if (a < 0 && b < std::numeric_limits<T>::min() - a) {
        return {};
    }
    return a + b;
}

template <typename T, size_t N>
[[gnu::const, nodiscard]]
constexpr std::array<T, N> convert_to_array(const std::vector<T>& v) {
    std::array<T, N> a;
    std::copy(std::begin(v), std::end(v), std::begin(a));
    return a;
}

template <typename T>
[[gnu::const, nodiscard]]
constexpr std::vector<T> get_all_fibonachi_until_overflow_impl() {
    std::vector<T> v = {0, 1};
    constexpr auto safe_add_wrap = [] (std::vector<T>& v) constexpr {
    return safe_add<T>(*std::rbegin(v), *std::next(std::rbegin(v)));
    };
    for (std::optional<T> o =
             safe_add_wrap(v);
         o.has_value();
         o = safe_add_wrap(v)) {
        v.push_back(o.value());
    }
    return v;
}



template <typename T, size_t N>
[[gnu::const, nodiscard]]
constexpr std::array<T, N> get_all_fibonachi_until_overflow() {
    return convert_to_array<T,
                            get_all_fibonachi_until_overflow_impl<T>().size()>(
        get_all_fibonachi_until_overflow_impl<T>());
}



int main(int argc, const char* const* argv) {
    using type = unsigned long long;
    constexpr auto count = get_all_fibonachi_until_overflow_impl<type>().size();
    constexpr auto values = get_all_fibonachi_until_overflow<type, count>();

    PrintNum(10);

    std::copy(std::begin(values), std::end(values),
              std::ostream_iterator<PrintNum<type>>(std::cout, "\n"));
    return count;
}