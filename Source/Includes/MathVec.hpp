#ifndef MATH_VEC_HPP
#define MATH_VEC_HPP

#include <concepts>
#include <utility>
#include <memory>
#include <algorithm>
#include <initializer_list>

#include "Meta/Helpers.hpp"

namespace SampleLib {
    template <int Fallback>
    constexpr int passCheckedValue(int value) {
        return (value >= Fallback)
            ? value
            : Fallback;
    }

    template <typename T> requires (std::floating_point<Meta::naked_t<T>>)
    class VecN {
    private:
        using plain_num_t = Meta::naked_t<T>;
        using num_ref_t = plain_num_t&;
        using num_ptr_t = plain_num_t*;
        using num_cref_t = const plain_num_t&;

        static constexpr int default_sz = 2;
        std::unique_ptr<plain_num_t[]> m_data;
        int m_length;

    public:
        VecN()
        : m_data (std::make_unique<plain_num_t[]>(default_sz)), m_length {default_sz} {
            std::fill(m_data.get(), m_data.get() + m_length, plain_num_t {});
        }

        VecN(T filler_value, int length)
        : m_data (std::make_unique<plain_num_t[]>(passCheckedValue<default_sz>(length))), m_length {passCheckedValue<default_sz>(length)} {
            std::fill(m_data.get(), m_data.get() + m_length, filler_value);
        }

        VecN& operator=(std::initializer_list<plain_num_t> items) {
            std::size_t pending_n = static_cast<std::size_t>(passCheckedValue<default_sz>(m_length));
            num_ptr_t dest_ptr = m_data.get();

            for (auto item_it = items.begin(); item_it != items.end() && pending_n > 0; item_it++, dest_ptr++, pending_n--) {
                *dest_ptr = *item_it;
            }

            return *this;
        }

        [[nodiscard]] int getLength() const { return m_length; }

        num_ref_t operator[](int pos) & noexcept {
            return m_data.get()[pos];
        }

        num_cref_t operator[](int pos) const& noexcept {
            return m_data.get()[pos];
        }
    };
}

#endif