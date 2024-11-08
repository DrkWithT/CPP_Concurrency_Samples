#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <type_traits>

namespace SampleLib::Meta {
    template <typename RawTp>
    using naked_t = std::remove_reference_t<std::remove_cv_t<RawTp>>;

    template <int Fallback>
    constexpr int passCheckedValue(int value) {
        return (value >= Fallback)
            ? value
            : Fallback;
    }
}

#endif