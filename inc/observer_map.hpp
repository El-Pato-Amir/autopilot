#ifndef OBSERVER_MAP_HPP
#define OBSERVER_MAP_HPP

#include "observer.hpp"
#include "blocking_map.hpp"
#include "blocking_queue.hpp"

#include <memory> //shared ptr

namespace fgear {

template <typename Value>
class MapObserver : public Observer {
    

private:
    std::shared_ptr<concurrency::BlockingMap<std::string, Value>> m_map;
    thread::BlockingQueue<std::string> m_updates;
    //class Tcp mediator
};

} // namespace fgear

#endif // OBSERVER_MAP_HPP