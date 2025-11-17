/**
 * @file PingPong.cpp
 * @author DrkWithT
 * @brief Implements two threads that take turns sending a message to the other until N rounds have passed.
 * @version 0.1.0
 * @date 11-15-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <chrono>
#include <atomic>
#include <thread>
#include <iostream>

constexpr auto pause_seconds = 1;
constexpr auto max_rounds = 5;

struct MessageState {
    std::atomic_int pending_rounds;
    std::atomic_flag is_pong;
};

int main() {
    MessageState shared_msg { .is_pong = true, .pending_rounds = max_rounds};

    // Thread 1: Send 'Ping' msg to Thread 2 after a 1s delay. Resumes when action is changed, yet ends on a pending round count of 0.
    auto runnable_1 = [&]() {
        while (shared_msg.pending_rounds.load() > 0) {
            std::this_thread::sleep_for(std::chrono::seconds {pause_seconds});

            if (shared_msg.is_pong.test()) {
                std::cout << "T1: PING\n";

                shared_msg.is_pong.clear();
                shared_msg.is_pong.notify_all();
            }
        }

        std::cout << "T1: FINISHED!\n";
    };

    // Thread 2: Send 'Pong' msg to Thread 1 after a 1s delay. Resumes when action is changed, yet ends on a pending round count of 0.
    auto runnable_2 = [&]() {
        while (shared_msg.pending_rounds.load() > 0) {
            if (!shared_msg.is_pong.test()) {
                std::cout << "T2: PONG\n";

                shared_msg.pending_rounds.fetch_sub(1);
                shared_msg.pending_rounds.notify_all();

                shared_msg.is_pong.test_and_set();
                shared_msg.is_pong.notify_all();
            }

            std::this_thread::sleep_for(std::chrono::seconds {pause_seconds});
        }

        std::cout << "T2: FINISHED!\n";
    };

    std::thread t1 {runnable_1};
    std::thread t2 {runnable_2};

    t2.join();
    t1.join();
}
