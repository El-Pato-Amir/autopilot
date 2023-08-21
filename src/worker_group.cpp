#include "worker_group.hpp"

namespace concurrency {

class PoisonApple : public std::exception {
public:
	PoisonApple() = default;
	const char* what() const noexcept override
	{
		return "You're poison, running through my vein";
	}
};

class FinisherTask : public Task {
public:
    FinisherTask() noexcept = default;
    ~FinisherTask() = default;

    void operator()() override { throw PoisonApple{}; };

};

Worker::Worker(thread::BlockingQueue<std::unique_ptr<Task>>& a_queue, std::vector<std::thread::id>& a_ids, std::atomic_bool& a_shutdown)
: m_queue{a_queue}
, m_deathrow{a_ids}
, m_shutdown{a_shutdown}
, m_remove{false}
{
}

void Worker::operator()()
{
    while (not m_shutdown) {
        std::unique_ptr<Task> task;
        m_queue.dequeue(task);
        if (not execute_task(task)) {
            return;
        }

        if (m_remove) {
            //TODO: this is not good! needs to be atomic
            m_deathrow.push_back(std::this_thread::get_id());
            return;
        }
    }
}

bool Worker::execute_task(std::unique_ptr<Task>& a_task)
{
    try {
        a_task.get()->operator()();
    } catch (PoisonApple const&) {
        return false;
    } catch(...) {
        //TODO:handle exceptions
        return true;
    }
    return true;    
}


WorkerGroup::WorkerGroup(size_t const& a_initial_threads, thread::BlockingQueue<std::unique_ptr<Task>>& a_queue)
: m_workers{}
, m_queue{a_queue}
, m_deathrow{}
, m_cv{}
, m_mutex{}
, m_shutdown{false}
{
    m_workers.reserve(a_initial_threads);
    //TODO: do this with add
    for (size_t i = 0; i < a_initial_threads; ++i) {
        std::thread worker{Worker{m_queue,m_deathrow,m_shutdown}};
        m_workers[worker.get_id()] = std::move(worker);
    }
}

WorkerGroup::~WorkerGroup()
{
    for (size_t i = 0; i < m_workers.size(); ++i) {
        m_queue.enqueue(std::make_unique<Task>(FinisherTask{}));
    }
    for (auto& it : m_workers) {
        it.second.join();
    }
}


} // concurrency
