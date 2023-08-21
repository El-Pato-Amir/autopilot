#include "thread_pool.hpp"

#include <exception> // exception class
#include <cassert> // assert

namespace concurrency {

class ThreadPoolExecuter::Worker {
public:

    explicit Worker(thread::BlockingQueue<std::unique_ptr<Task>>& a_queue,std::atomic_bool& a_shutdown,std::atomic_bool& a_remove, MiddleMan& a_middle_man);
    ~Worker() = default;

    void operator()();

private:
    bool execute_task(std::unique_ptr<Task>& a_task);

private:
    thread::BlockingQueue<std::unique_ptr<Task>>& m_queue;
    std::atomic_bool& m_shutdown;
    std::atomic_bool& m_remove;
};

ThreadPoolExecuter::Worker::Worker(thread::BlockingQueue<std::unique_ptr<Task>>& a_queue,std::atomic_bool& a_shutdown,std::atomic_bool& a_remove, MiddleMan& a_middle_man)
: m_queue{a_queue}
, m_shutdown{a_shutdown}
, m_remove{a_remove}
{
}

void ThreadPoolExecuter::Worker::operator()()
{
    while (not m_shutdown) {
        std::unique_ptr<Task> task;
        m_queue.dequeue(task);
        if (not execute_task(task)) {
            return;
        }
        if (m_remove) {
            if (m_middle_man.join_death_row(std::this_thread::get_id())) {
                return;
            }
        }
    }
}

bool ThreadPoolExecuter::Worker::execute_task(std::unique_ptr<Task>& a_task)
{
    try {
        a_task.get()->operator()();
    } catch (ThreadPoolExecuter::PoisonApple const&) {
        return false;
    } catch(...) {
        //do something
        return true;
    }
    return true;    
}


class ThreadPoolExecuter::FinisherTask : public Task {
public:
    FinisherTask() noexcept = default;
    ~FinisherTask() = default;

    void operator()() override;

};

void ThreadPoolExecuter::FinisherTask::operator()()
{
    throw PoisonApple{};
}

ThreadPoolExecuter::ThreadPoolExecuter(size_t a_num_of_threads,size_t a_queue_size)
: m_queue{a_queue_size}
, m_threads{}
, m_cv{}
, m_mutex{}
, m_immediate_shutdown{false}
, m_shutdown{false}
, m_remove{}
{
    m_threads.reserve(a_num_of_threads);
    std::unique_lock lock{m_mutex};
    for (size_t i = 0; i < a_num_of_threads; ++i) {
        m_threads.emplace_back(std::thread(Worker{m_queue,m_immediate_shutdown,m_remove,m_middle_man}));
    }
}

ThreadPoolExecuter::~ThreadPoolExecuter()
{
    std::unique_lock lock{m_mutex};
    if (not m_immediate_shutdown) {
        for (size_t i = 0; i < m_threads.size(); ++i) {
            m_queue.enqueue(std::make_unique<FinisherTask>(FinisherTask{}));
        }
    }

    for (auto& thread : m_threads) {
        thread.join();
    }
}

void ThreadPoolExecuter::submit(std::unique_ptr<Task>&& a_task)
{
    if (m_shutdown) {
        return;
    }
    m_queue.enqueue(std::move(a_task));
    m_cv.notify_all();
}

size_t ThreadPoolExecuter::threads() const
{
    std::unique_lock lock{m_mutex};
    return m_threads.size();
}


void ThreadPoolExecuter::shutdown()
{
    std::unique_lock lock{m_mutex};
    m_shutdown = true;
    for (size_t i = 0; i < m_threads.size(); ++i) {
        m_queue.enqueue(std::make_unique<FinisherTask>(FinisherTask{}));
    }
    for (auto& thread : m_threads) {
        thread.join();
    }
    m_threads.clear();
    m_cv.notify_all();
}

void ThreadPoolExecuter::add(size_t a_threads)
{
    std::unique_lock lock{m_mutex};
    for (size_t i = 0; i < a_threads; ++i) {
        m_threads.emplace_back(std::thread(Worker{m_queue,m_immediate_shutdown,m_remove,m_middle_man}));
    }
    m_cv.notify_all();
}

void ThreadPoolExecuter::remove(size_t a_threads)
{
    std::unique_lock lock{m_mutex};
    assert(a_threads <= m_threads.size());
    if (a_threads > m_threads.size()) {
        return;
    }
    m_middle_man.m_wanted = a_threads;
    m_remove = true;

    for (size_t i = 0; i < a_threads; ++i) {
        m_queue.enqueue(std::make_unique<FinisherTask>(FinisherTask{}));
    }
    while (m_middle_man.m_ids.size() < a_threads) {
        m_cv.wait(lock);
    }
    m_remove = false;
    auto remove_func = [this]() { remove_threads();};
    std::thread remove{remove_func};
    while (m_middle_man.m_ids.size() > 0) {
        m_cv.wait(lock);
    }
    remove.join();
    m_cv.notify_all();    
}

void ThreadPoolExecuter::remove_threads()
{
    std::unique_lock lock{m_mutex};
    for (size_t i = 0; i < m_middle_man.m_ids.size(); ++i) {
        auto begin = m_threads.begin();
        auto end = m_threads.end();
        while (begin != end) {
            if (begin->get_id() == m_middle_man.m_ids.at(i)) {
                break;
            }
            ++begin;
        }
    //TODO:maybe swap with last element to make more efficient
        begin->join();
        m_threads.erase(begin);
    }
    m_middle_man.m_ids.clear();
    m_cv.notify_all();
}

void ThreadPoolExecuter::immediate_shutdown()
{
    std::unique_lock lock{m_mutex};
    m_shutdown = true;
    m_immediate_shutdown = true;
    for (size_t i = 0; i < m_threads.size(); ++i) {
        m_queue.enqueue(std::make_unique<FinisherTask>(FinisherTask{}));
    }
    for (auto& thread : m_threads) {
        thread.join();
    }
    m_threads.clear();
    m_cv.notify_all();           
}


} // namespace concurrency
