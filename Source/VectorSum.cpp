/**
 * @file VectorSum.cpp
 * @author DrkWithT
 * @brief Implements threaded math-vector sum.
 * @version 0.1.0
 * @date 2024-11-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <thread>
#include <iostream>
#include "MathVec.hpp"

static constexpr int vec_length = 8;
static constexpr int thread_count = 2;

using RealVec = SampleLib::VecN<double>;

void printRealVec(const RealVec& arg) {
    int len = arg.getLength();

    for (int i = 0; i < len; i++) {
        std::cout << arg[i] << ' ';
    }

    std::cout << '\n';
}

int main() {
    RealVec vec_1 {0.0, vec_length};
    RealVec vec_2 {0.0, vec_length};
    RealVec result {0.0, vec_length};

    vec_1 = {1, 2, 3, 4, 5, 6, 7, 8};
    vec_2 = {8, 7, 6, 5, 4, 3, 2, 1};

    auto parallel_vecsum = [&](int rank){
        constexpr int local_count = vec_length / thread_count; 
        int local_idx = local_count * rank;
        int end = local_idx + local_count;

        for (; local_idx < end; local_idx++) {
            result[local_idx] = vec_1[local_idx] + vec_2[local_idx];
        }
    };

    std::thread t1 {parallel_vecsum, 0};
    std::thread t2 {parallel_vecsum, 1};

    t2.join();
    t1.join();

    printRealVec(result);
}
