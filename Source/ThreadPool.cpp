/**
 * @file ThreadPool.cpp
 * @author DrkWithT
 * @brief Implements simple thread pool.
 * @version 0.1.0
 * @date 2024-12-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <vector>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>

#include <chrono>
#include <random>
#include <string>
#include <iostream>

/* Misc. Types */

constexpr std::size_t min_delay = 1;
constexpr std::size_t max_delay = 3;
constexpr std::size_t min_item = 1;
constexpr std::size_t max_item = 10;

struct NumTask {
    std::size_t item;
    bool poison;
};

/* Thread pool defs. */

class ThreadPool {
private:
    static constexpr std::size_t default_workers_n = 2;

    std::vector<std::thread> m_threads;
    std::queue<NumTask> m_tasks;
    std::mutex m_tasks_mtx;
    std::mutex m_cv_mtx;
    std::condition_variable m_task_cv;
    std::atomic_flag m_stop_flag;

public:
    explicit ThreadPool()
    : m_threads {}, m_tasks {}, m_tasks_mtx {}, m_cv_mtx {}, m_task_cv {}, m_stop_flag {false} {}

    void runTasks() {
        auto producer_fn = [this]() {
            constexpr std::size_t item_range = max_item - min_item;
            std::mt19937 randomizer;

            while (not m_stop_flag.test()) {
                const auto temp_item = randomizer() % item_range + min_item;

                {
                std::unique_lock tasks_lock {m_tasks_mtx};

                m_tasks.push(NumTask {temp_item, false});
                }

                m_task_cv.notify_one();

                std::this_thread::sleep_for(std::chrono::seconds {2});
            }

            {
            // place poison task to stop workers
            std::unique_lock tasks_lock_2 {m_tasks_mtx};

            for (std::size_t worker_count = 0; worker_count < default_workers_n; worker_count++) {
                m_tasks.push(NumTask {0, true});
            }
            }

            m_task_cv.notify_all();

            std::cout << "Producer stopped.\n";
        };

        auto worker_fn = [this](std::size_t rank) {
            constexpr std::size_t delay_range = max_delay - min_delay;
            std::mt19937 randomizer;
            NumTask temp {};

            while (true) {
                {
                std::unique_lock cv_lock {m_cv_mtx};
                m_task_cv.wait(cv_lock, [this]() {
                    return m_tasks.size() > 0;
                });
                }

                {
                std::unique_lock tasks_lock {m_tasks_mtx};
                temp = m_tasks.front();
                m_tasks.pop();
                }

                if (temp.poison || temp.item == 0) {
                    break;
                }

                std::cout << "Worker " << rank << " processed " << temp.item << '\n';

                /// Simulate delay between "work".
                const auto next_delay = min_delay + randomizer() % delay_range;
                std::this_thread::sleep_for(std::chrono::seconds {next_delay});
            }

            std::cout << "Worker " << rank << " stopped.\n";
        };

        m_threads.emplace_back(std::thread {producer_fn});
        
        for (std::size_t worker_rank = 0; worker_rank < default_workers_n; worker_rank++) {
            m_threads.emplace_back(std::thread {worker_fn, worker_rank});
        }

        std::string user_input;
        while (true) {
            std::cout << "Enter QUIT to stop the pool.\n";
            std::getline(std::cin, user_input);

            if (user_input == "QUIT") {
                break;
            }

            user_input.clear();
        }

        haltTasks();
    }

    void haltTasks() {
        std::cout << "Stopping pool...\n";

        m_stop_flag.test_and_set();
        m_stop_flag.notify_one();

        for (auto& thrd : m_threads) {
            thrd.join();
        }
    }
};

int main() {
    ThreadPool my_pool {};

    my_pool.runTasks();
}
