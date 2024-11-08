/**
 * @file MatrixVectorMult.cpp
 * @author DrkWithT
 * @brief Implements Matrix by vector multiplication program.
 * @version 0.1.0
 * @date 2024-11-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <vector>
#include <thread>
#include <iostream>
#include "Meta/Helpers.hpp"
#include "MathMatrix.hpp"
#include "MathVec.hpp"

using RealMatrix = SampleLib::Matrix<double>;
using RealVector = SampleLib::VecN<double>;

void fillLinearObject(RealVector& vec) {
    const int len = vec.getLength();
    double temp {};

    std::cout << "Enter vector of size " << len << ":\n";

    for (int vec_i = 0; vec_i < len; vec_i++) {
        std::cin >> temp;
        vec[vec_i] = temp;
    }
}

void fillLinearObject(RealMatrix& mat) {
    const int side_len = mat.getSideLength();
    double temp {};

    for (int row_i = 0; row_i < side_len; row_i++) {
        std::cout << "Enter row " << row_i << ":\n";

        for (int col_i = 0; col_i < side_len; col_i++) {
            std::cin >> temp;
            mat[{row_i, col_i}] = temp;
        }
    }
}

void showLinearObject(const RealVector& vec) {
    const int len = vec.getLength();

    for (int i = 0; i < len; i++) {
        std::cout << vec[i] << ' ';
    }

    std::cout << '\n';
}

int main() {
    int dimension_n = 1;
    int thread_count = 1;

    std::cout << "Enter N (should be positive):\n";
    std::cin >> dimension_n;

    std::cout << "Enter thread count (should be positive)\n";
    std::cin >> thread_count;

    RealMatrix mat_a {dimension_n};
    RealVector vec_x {0.0, dimension_n};
    RealVector vec_y {0.0, dimension_n};

    // get user inputted matrix and vector...

    fillLinearObject(mat_a);
    fillLinearObject(vec_x);

    auto parallel_mv_multiply= [&](int rank){
        const int local_rows = dimension_n / thread_count;
        const int first_row = rank * local_rows;
        const int last_row = first_row + local_rows - 1;

        for (int row_i = first_row; row_i <= last_row; row_i++) {
            for (int col_i = 0; col_i < dimension_n; col_i++) {
                vec_y[row_i] += mat_a[{row_i, col_i}] * vec_x[col_i];
            }
        }
    };

    std::vector<std::thread> threads (thread_count);

    for (int thrd_rank = 0; thrd_rank < thread_count; thrd_rank++) {
        threads[thrd_rank] = std::thread {parallel_mv_multiply, thrd_rank};
    }

    for (int thrd_idx = 0; thrd_idx < thread_count; thrd_idx++) {
        threads[thrd_idx].join();
    }

    /// Show results!
    showLinearObject(vec_y);
}