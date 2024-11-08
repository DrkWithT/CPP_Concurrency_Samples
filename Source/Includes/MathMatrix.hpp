#ifndef MATH_MATRIX_HPP
#define MATH_MATRIX_HPP

#include <concepts>
#include <memory>
#include <algorithm>

#include "Meta/Helpers.hpp"

namespace SampleLib {
    using Meta::passCheckedValue;

    struct MatrixPos {
        int row;
        int col;
    };

    template <typename T> requires (std::floating_point<Meta::naked_t<T>>)
    class Matrix {
    private:
        using plain_num_t = Meta::naked_t<T>;
        using num_ref_t = plain_num_t&;
        using num_ptr_t = plain_num_t*;
        using num_cref_t = const plain_num_t&;

        static constexpr int default_sz = 2;
        std::unique_ptr<plain_num_t[]> m_data;
        int m_side;

    public:
        Matrix()
        : m_data (std::make_unique<plain_num_t[]>(default_sz * default_sz)), m_side {default_sz} {
            std::fill(m_data.get(), m_data.get() + default_sz + default_sz);
        }

        Matrix(int side)
        : m_data (std::make_unique<plain_num_t[]>(side * side)), m_side{passCheckedValue<default_sz>(side)} {
            const int checked_side = passCheckedValue<default_sz>(side);
            std::fill(m_data.get(), m_data.get() + checked_side * checked_side, plain_num_t {});
        }

        [[nodiscard]] int getSideLength() const { return m_side; }

        num_ref_t operator[](MatrixPos pos) & noexcept {
            auto [row, col] = pos;

            return m_data.get()[row * getSideLength() + col];
        }

        num_cref_t operator[](MatrixPos pos) const& noexcept {
            auto [row, col] = pos;

            return m_data.get()[row * getSideLength() + col];
        }
    };
}

#endif