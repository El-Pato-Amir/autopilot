#ifndef TASK_HPP
#define TASK_HPP

namespace concurrency {

class Task {
public:
    virtual ~Task() = default;

    virtual void operator()() = 0;

protected:
    Task() = default;
    Task(Task const&) = default;
    Task& operator=(Task const&) = default; 
};

} // namespace concurrency

#endif // TASK_HPP