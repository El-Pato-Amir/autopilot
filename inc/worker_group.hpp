#ifndef WORKER_GROUP_HPP
#define WORKER_GROUP_HPP

#include "blocking_queue.hpp"
#include "task.hpp"

#include <atomic> //atomic
#include <vector> // deathrow vector
#include <unordered_map> // thread_id map
#include <thread> //threads
#include <condition_variable> // condiotional variables

namespace concurrency {

class Worker {
public:
    explicit Worker(thread::BlockingQueue<std::unique_ptr<Task>>& a_queue, std::vector<std::thread::id>& a_ids);
    ~Worker() = default;

    void operator()();

private:
    bool execute_task(std::unique_ptr<Task>& a_task);

private:
    thread::BlockingQueue<std::unique_ptr<Task>>& m_queue;
    std::vector<std::thread::id>& m_deathrow;
    //TODO: maybe we don't need these atomics anymore
    std::atomic_bool m_shutdown;
    std::atomic_bool m_remove;    
};

class WorkerGroup {
public:
    WorkerGroup(size_t const& a_initial_threads);
    ~WorkerGroup();

private:
    std::unordered_map<std::thread::id, std::thread> m_workers;
    std::vector<std::thread::id> m_deathrow;
    std::condition_variable m_cv;
    mutable std::mutex m_mutex; 
};

} // namespace concurrency

#endif // WORKER_GROUP_HPP