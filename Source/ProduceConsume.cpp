/**
 * @file ProduceConsume.cpp
 * @author DrkWithT
 * @brief This producer & consumer sample accumulates a sum of random numbers.
 * @version 0.1.0
 * @date 2024-11-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <chrono>
#include <ctime>
#include <random>

#include <atomic>
#include <thread>
#include <mutex>
#include <csignal>

#include <iostream>

static constexpr unsigned int max_item = 10;
static constexpr unsigned int min_item = 1;

std::atomic_flag halt_all {false};
std::atomic_flag temp_ready {false};
std::mutex mtx {};

[[nodiscard]] static unsigned int getRandomInt(std::mt19937& num_engine, unsigned int min, unsigned int max) {
    unsigned int n = num_engine();
    return min + n % (max - min);
}

void onSigInt([[maybe_unused]] int s_code) {
    halt_all.test_and_set();
}

int main() {
    signal(SIGINT, onSigInt);

    std::mt19937 rand_engine {};
    rand_engine.seed(time(nullptr));

    unsigned int total = 0;
    unsigned int temp = 0;

    auto producer_fn = [&]() {
        while (true) {
            if (halt_all.test()) {
                std::lock_guard<std::mutex> lock {mtx};
                temp = 0;
                temp_ready.test_and_set();
                temp_ready.notify_all();

                break;
            }

            if (!temp_ready.test()) {
                std::lock_guard<std::mutex> lock2 {mtx};

                temp = getRandomInt(rand_engine, min_item, max_item);
                std::cout << "producing: " << temp << '\n';

                temp_ready.test_and_set();
                temp_ready.notify_all();
            }
        }
    };

    auto consumer_fn = [&]() {
        while (true) {
            if (temp == 0) {
                break;
            }

            std::chrono::milliseconds pause_duration {1000};
            std::this_thread::sleep_for(pause_duration);

            if (temp_ready.test()) {
                std::lock_guard<std::mutex> lock {mtx};
                std::cout << "consuming: " << temp << '\n';
                total += temp;
                temp_ready.clear();
                temp_ready.notify_all();
            }
        }
    };

    std::thread t1 {producer_fn};
    std::thread t2 {consumer_fn};

    t1.join();
    t2.join();

    std::cout << "total = " << total << '\n';
}
