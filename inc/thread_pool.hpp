#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include "blocking_queue.hpp"
#include "task.hpp"

#include <cstddef> // size_t
#include <vector> // vector
#include <memory> // unqiue pointers
#include <atomic> // atomic variables
#include <mutex> // mutex

//TODO: worker group
namespace concurrency {

class ThreadPoolExecuter {
public:
    // default queue size is 128
    // default number of threads is number of cores in computer - 1 (or 3 if can't infer cores)
    ThreadPoolExecuter();

    //TODO: do defaulted queue size
    explicit ThreadPoolExecuter(size_t a_num_of_threads);
    explicit ThreadPoolExecuter(size_t a_num_of_threads,size_t a_queue_size);

    ~ThreadPoolExecuter();

    ThreadPoolExecuter(ThreadPoolExecuter&&) = delete;
    ThreadPoolExecuter& operator=(ThreadPoolExecuter&&) = delete;

    void submit(std::unique_ptr<Task>&& a_task);

    size_t threads() const;

    void add(size_t a_threads);

    void remove(size_t a_threads);

    void shutdown();
    void immediate_shutdown();

private:
    void remove_threads();

private:
	class Worker;
	class FinisherTask;
	class PoisonApple : public std::exception {
	public:
		PoisonApple() = default;
		const char* what() const noexcept override
		{
			return "You're poison, running through my vein";
		}
	};

private:
    thread::BlockingQueue<std::unique_ptr<Task>> m_queue;
    std::vector<std::thread> m_threads;
    std::condition_variable m_cv;
    mutable std::mutex m_mutex;
    std::atomic_bool m_immediate_shutdown;
    std::atomic_bool m_shutdown;
    std::atomic_bool m_remove;
};


} // namespace concurrency

#endif // THREAD_POOL_HPP
