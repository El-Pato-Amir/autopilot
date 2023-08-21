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
    explicit Worker(thread::BlockingQueue<std::unique_ptr<Task>>& a_queue, std::vector<std::thread::id>& a_ids, std::atomic_bool& a_shutdown);
    ~Worker() = default;

    void operator()();

private:
    bool execute_task(std::unique_ptr<Task>& a_task);

private:
    thread::BlockingQueue<std::unique_ptr<Task>>& m_queue;
    std::vector<std::thread::id>& m_deathrow;
    //TODO: maybe add a mutex variable
    std::atomic_bool& m_shutdown;
    std::atomic_bool m_remove;    
};

class WorkerGroup {
public:
    WorkerGroup(size_t const& a_initial_threads, thread::BlockingQueue<std::unique_ptr<Task>>& a_queue);
    ~WorkerGroup();

    void add_workers(size_t const& a_number);

private:
    std::unordered_map<std::thread::id, std::thread> m_workers;
    thread::BlockingQueue<std::unique_ptr<Task>>& m_queue;
    std::vector<std::thread::id> m_deathrow;
    std::condition_variable m_cv;
    mutable std::mutex m_mutex;
    std::atomic_bool m_shutdown; 
};

} // namespace concurrency

#endif // WORKER_GROUP_HPP