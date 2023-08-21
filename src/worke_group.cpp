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

Worker::Worker(thread::BlockingQueue<std::unique_ptr<Task>>& a_queue, std::vector<std::thread::id>& a_ids)
: m_queue{a_queue}
, m_deathrow{a_ids}
, m_shutdown{false}
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

} // concurrency
