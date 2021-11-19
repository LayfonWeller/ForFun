#include <iostream>
#include <type_traits>
#include <concepts>

template <std::integral T>
struct PrintNum {
    T num;

    constexpr PrintNum(T a) : num(a){}
    constexpr PrintNum& operator=(const T a) {
        this->num = a;
        return *this;
    }
    constexpr PrintNum(const PrintNum<T>&a) = default;

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