#include <algorithm>
#include <array>
#include <concepts>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <numeric>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

template <typename T>
[[gnu::const, nodiscard]] constexpr std::optional<T> safe_add(T a, T b) {
    if (a > 0 && b > std::numeric_limits<T>::max() - a) {
        return std::nullopt;
    } else if (a < 0 && b < std::numeric_limits<T>::min() - a) {
        return std::nullopt;
    }
    return a + b;
}

template <typename T, size_t N>
[[gnu::const, nodiscard]] constexpr std::array<T, N> convert_to_array(
    const std::vector<T>& v) {
    std::array<T, N> a;
    std::copy(std::begin(v), std::end(v), std::begin(a));
    return a;
}

template <typename T>
[[gnu::const, nodiscard]] constexpr std::vector<T>
get_all_fibonachi_until_overflow_impl() {
    std::vector<T> v = {0, 1};
    constexpr auto safe_add_wrap = [](std::vector<T> & v) constexpr {
        return safe_add<T>(*std::rbegin(v), *std::next(std::rbegin(v)));
    };
    for (std::optional<T> o = safe_add_wrap(v); o.has_value();
         o = safe_add_wrap(v)) {
        v.push_back(o.value());
    }
    return v;
}

template <typename T, size_t N>
[[gnu::const, nodiscard]] constexpr std::array<T, N>
get_all_fibonachi_until_overflow() {
    return convert_to_array<T,
                            get_all_fibonachi_until_overflow_impl<T>().size()>(
        get_all_fibonachi_until_overflow_impl<T>());
}

template <std::integral T>
struct PrintNum {
    T num;

    constexpr PrintNum(T a) : num(a) {}
    constexpr PrintNum& operator=(const T a) {
        this->num = a;
        return *this;
    }
    constexpr PrintNum(const PrintNum<T>& a) = default;

    constexpr PrintNum(PrintNum<T>&& a) = default;
    constexpr PrintNum& operator=(PrintNum&& a) = default;
};

template <std::integral T>
inline std::ostream& operator<<(std::ostream& os, PrintNum<T> c) {
    return os << c.num;
}

template <>
inline std::ostream& operator<<(std::ostream& os, PrintNum<char> c) {
    if constexpr (std::is_unsigned_v<char>)
        return os << static_cast<unsigned int>(c.num);
    else
        return os << static_cast<int>(c.num);
}

template <>
inline std::ostream& operator<<(std::ostream& os, PrintNum<uint8_t> c) {
    return os << static_cast<unsigned int>(c.num);
}

template <>
inline std::ostream& operator<<(std::ostream& os, PrintNum<int8_t> c) {
    return os << static_cast<int>(c.num);
}

// Hasher https://stackoverflow.com/questions/2111667/compile-time-string-hashing
namespace my_hash {
template <class>
struct hasher;
template <>
struct hasher<std::string> {
    std::size_t constexpr operator()(char const* input) const {
        return *input
                   ? static_cast<unsigned int>(*input) + 33 * (*this)(input + 1)
                   : 5381;
    }
    std::size_t operator()(const std::string& str) const {
        return (*this)(str.c_str());
    }
};
template <>
struct hasher<std::string_view> {
    std::size_t constexpr operator()(char const* const input,
                                     const size_t len) const {
        return len > 0 ? static_cast<unsigned int>(*input) +
                             33 * (*this)(input + 1, len - 1)
                       : 5381;
    }
    std::size_t operator()(const std::string_view& str) const {
        return (*this)(str.data(), str.length());
    }
};
template <>
struct hasher<const char*> {
    std::size_t constexpr operator()(char const* const input,
                                     const size_t len) const {
        return len > 0 ? static_cast<unsigned int>(*input) +
                             33 * (*this)(input + 1, len - 1)
                       : 5381;
    }
    std::size_t constexpr operator()(char const* const input) const {
        return *input > 0
                   ? static_cast<unsigned int>(*input) + 33 * (*this)(input + 1)
                   : 5381;
    }
};
template <typename T>
std::size_t constexpr hash(T&& t) {
    return hasher<typename std::decay<T>::type>()(std::forward<T>(t));
}
inline namespace literals {
std::size_t constexpr operator"" _hash(const char* const s, const size_t len) {
    return hasher<std::string_view>()(s, len);
}
}  // namespace literals
}  // namespace my_hash

template <std::integral T>
constexpr auto getFibonachi() {
    using type = T;
    constexpr auto count = get_all_fibonachi_until_overflow_impl<type>().size();
    constexpr auto values = get_all_fibonachi_until_overflow<type, count>();

    return values;
}

template <std::ranges::input_range T, typename PrintFormat = PrintNum<std::ranges::range_value_t<T>>>
void printArray(const T& v) {
    // std::ranges::copy(
    //     v, std::ostream_iterator<PrintFormat>(
    //            std::cout, "\n"));
}

int main(int argc, const char* const* argv) {
    if (argc >= 2) {
        using namespace my_hash::literals;
        switch (my_hash::hash(argv[1])) {
            case "uint8_t"_hash: {
                static constexpr auto v = getFibonachi<uint8_t>();
                printArray(v);
                return v.size();
            }
            case "char"_hash: {
                static constexpr auto v = getFibonachi<char>();
                printArray(v);
                return v.size();
            }
            case "int8_t"_hash: {
                static constexpr auto v = getFibonachi<int8_t>();
                printArray(v);
                return v.size();
            }
            case "int16_t"_hash: {
                static constexpr auto v = getFibonachi<int16_t>();
                printArray(v);
                return v.size();
            }
            case "uint16_t"_hash: {
                static constexpr auto v = getFibonachi<uint16_t>();
                printArray(v);
                return v.size();
            }
            case "int32_t"_hash: {
                static constexpr auto v = getFibonachi<int32_t>();
                printArray(v);
                return v.size();
            }
            case "uint32_t"_hash: {
                static constexpr auto v = getFibonachi<uint32_t>();
                printArray(v);
                return v.size();
            }
            case "int64_t"_hash: {
                static constexpr auto v = getFibonachi<int64_t>();
                printArray(v);
                return v.size();
            }
            case "uint64_t"_hash: {
                static constexpr auto v = getFibonachi<uint64_t>();
                printArray(v);
                return v.size();
            }
        };
    }
}