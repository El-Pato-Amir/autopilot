#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <string> // std::string

namespace fgear {

template<typename Key>
class Observer {
public:
    virtual ~Observer() = default;

    virtual void notify(Key const& a_message) = 0;

protected:
    Observer() = default;
    Observer(Observer const&) = default;
    Observer& operator=(Observer const&) = default;
};

} // namespace fgear

#endif // OBSERVER_HPP